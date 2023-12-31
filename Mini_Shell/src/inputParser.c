#include <stdio.h>
#include <string.h>

#include "../include/inputParser.h"
#include "../include/commandManager.h"

Command *parseInput(const char *input)
{
    char *inputCopy = strdup(input);

    //char *newline = strchr(inputCopy, '\n');
    //if (newline)
    //    *newline = 0;

    char *token = strtok(inputCopy, " ");
    // Extract command
    char *executable = token;

    // Because there is a LF at the ned of executable and i'm bored
    size_t len = strlen(executable);
    if (executable[len - 1] == '\n')
    {
        executable[len - 1] = '\0';
    }

    // Extracting arguments
    int arg_count = 0;
    char **args = (char **)malloc(sizeof(char *));

    while (token != NULL)
    {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            args[arg_count] = strdup(token);
            arg_count++;
            args = realloc(args, (arg_count + 1) * sizeof(char *));
        }
    }

    // Creating command struct
    Command *cmd = createCommand(executable, args, arg_count, 0);

    // Debug print the Command structure
    printf("Command Structure: \n");
    printf("Executable: %s\n", executable);
    for (int i = 0; i < cmd->arg_count; i++)
    {
        printf("Argument %d: %s\n", i + 1, cmd->args[i]);
    }

    // Check for background execution symbol
    char *background_token = strstr(input, "&");
    if (background_token != NULL)
    {
        cmd->run_in_background = 1;
        *background_token = '\0'; // Remove the "&" Symbol
    }
    else
    {
        cmd->run_in_background = 0;
    }

    return cmd;
}