#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/commandManager.h"
#include "../include/cd.h"
#include "../include/ps.h"
#include "../include/exit.h"
#include "../include/ls.h"
#include "../include/pwdCmd.h"
#include "../include/echo.h"
#include "../include/history.h"
#include "../include/typedef.h"
#include <fcntl.h>

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
    {"ls", execute_ls},
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
    cmd->next_command = NULL;
    cmd->has_input_redirection = 0;
    cmd->input_file = NULL;
    cmd->has_output_redirection = 0;
    cmd->output_file = NULL;

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

void setupRedirection(Command *cmd)
{
    // Handle input redirection
    if (cmd->has_input_redirection)
    {
        int fd_in = open(cmd->input_file, O_RDONLY);
        if (fd_in == -1)
        {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    // Handle output redirection
    if (cmd->has_output_redirection)
    {
        int fd_out;

        // If append_output is true, open the file in append mode
        if (cmd->append_output)
        {
            fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        }
        else
        {
            // Otherwise, redirect to the specified file
            fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        }

        if (fd_out == -1)
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }

        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    // Handle here document content
    if (cmd->here_document_content)
    {
        // Open a temporary file to store the here document content
        char tmp_filename[] = "/tmp/minishell_here_doc_XXXXXX";
        int fd_tmp = mkstemp(tmp_filename);
        if (fd_tmp == -1)
        {
            perror("Error creating temporary file for here document");
            exit(EXIT_FAILURE);
        }

        // Write the content to the temporary file
        write(fd_tmp, cmd->here_document_content, strlen(cmd->here_document_content));
        lseek(fd_tmp, 0, SEEK_SET);

        // Redirect stdin to the temporary file
        dup2(fd_tmp, STDIN_FILENO);

        // Close the temporary file (it will be deleted automatically)
        close(fd_tmp);
    }
}

int executeCommand(Command *cmd)
{
    setupRedirection(cmd);
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

void restoreTerminal() {
    // Revert standard input to terminal
    int new_stdin = open("/dev/tty", O_RDONLY);
    dup2(new_stdin, STDIN_FILENO);
    close(new_stdin);

    // Revert standard output to terminal
    int new_stdout = open("/dev/tty", O_WRONLY);
    dup2(new_stdout, STDOUT_FILENO);
    close(new_stdout);
}

int executeBuiltInCommand(Command *cmd , CommandFunction function){
    function(cmd->args);
    restoreTerminal();
}

int executeOtherCommands(Command *cmd , CommandFunction function)
{
    if (cmd->has_pipe)
    {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            close(pipe_fd[0]); // Close unused read end
            dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
            close(pipe_fd[1]); // Close the write end of the pipe

            exit(function(cmd->args));
        }
        else
        {
            // Parent process
            close(pipe_fd[1]); // Close unused write end
            int status;
            waitpid(pid, &status, 0);

            // Recursively execute the next command in the pipeline
            if (cmd->next_command != NULL)
            {
                executeOtherCommands(cmd->next_command, function);
            }

            return WEXITSTATUS(status);
        }
    }
    else
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
}

void freeCommand(Command *cmd)
{
    free(cmd);
}