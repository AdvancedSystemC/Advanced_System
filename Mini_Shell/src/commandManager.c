#include "../include/commandManager.h"
#include "../include/commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

CommandTableEntry CommandTable[] = {
    {"ps", execute_ps},
    {"exit", execute_exit},
    {"cd", execute_cd},
    {"pwd", execute_pwd},
    {"echo", execute_ps},
    {"who", execute_echo},
    {"history", execute_history},
    //{"alias",execute_ps},
    //{"unalias",deleteAlias},
    //{"alias-load",loadAliases},
    //{"alias-save",saveAliases},
    {NULL, NULL} // Terminator for the table
};

Command *createCommand(char *executable, char **args, int arg_count, int has_pipe)
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    if (cmd == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    cmd->executable = executable;
    cmd->args = args;
    cmd->arg_count = arg_count;
    cmd->has_pipe = has_pipe;

    return cmd;
}

CommandFunction findCommandFunction(const char *cmd)
{
    for (int i = 0; CommandTable[i].name != NULL; i++)
    {
        if (strcmp(CommandTable[i].name, cmd) == 0)
        {
            printf("%s", CommandTable[i].name);
            return CommandTable[i].function;
        }
    }

    return NULL;
}

int executeCommand(Command *cmd)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        if (cmd->run_in_background)
        {
            if (setsid() == -1)
            {
                perror("setsid failed");
                exit(EXIT_FAILURE);
            }

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }

        CommandFunction commandFunction = findCommandFunction(cmd->executable);
        if (commandFunction != NULL)
        {
            exit(commandFunction(cmd->args));
        }
        else
        {
            fprintf(stderr, "Unknown command: %s\n", cmd->executable);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process

        if (!cmd->run_in_background)
        {
            int status;
            waitpid(pid, &status, 0);
            return WEXITSTATUS(status);
        }
        else
        {
            // Background execution
            return 0;
        }
    }
}

void freeCommand(Command *cmd)
{
    free(cmd);
}