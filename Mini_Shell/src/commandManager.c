#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/commandManager.h"
#include "../include/cd.h"
#include "../include/ps.h"
#include "../include/exit.h"
#include "../include/pwd.h"
#include "../include/echo.h"
#include "../include/history.h"
#include "../include/typedef.h"

CommandTableEntry BuiltInCommandTable[] = {
    {"exit",execute_exit},
    {"cd",execute_cd},
    {"pwd",execute_pwd},
    {"echo",execute_echo},
    {NULL,NULL} //Terminator for the table
};

CommandTableEntry CommandTable[] = {
    {"ps", execute_ps},
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

CommandFunction findCommandFunction(const char *cmd , CommandTableEntry *entry)
{
    for (int i = 0; entry[i].name != NULL; i++)
    {
        if (strcmp(entry[i].name, cmd) == 0)
        {
            return entry[i].function;
        }
    }

    return NULL;
}

int executeCommand(Command *cmd)
{
    CommandFunction commandFunction = findCommandFunction(cmd->executable,BuiltInCommandTable);
    if(commandFunction != NULL){
        executeBuiltInCommand(cmd,commandFunction);
    }
    else{
        commandFunction = findCommandFunction(cmd->executable,CommandTable);
        if (commandFunction != NULL)
        {
            executeOtherCommands(cmd,commandFunction);
        }
        else
        {
            fprintf(stderr, "Unknown command: %s\n", cmd->executable);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

int executeBuiltInCommand(Command *cmd , CommandFunction function){
    exit(function(cmd->args));
}

int executeOtherCommands(Command *cmd , CommandFunction function)
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

        exit(function(cmd->args));
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