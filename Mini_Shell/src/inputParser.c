#include <stdio.h>
#include <string.h>

#include "../include/inputParser.h"
#include "../include/commandManager.h"

Command *parseInput(const char *input)
{
    char *inputCopy = strdup(input);

    char *newline = strchr(inputCopy, '\n');
    if (newline)
        *newline = 0;

    char *token = strtok(inputCopy, " ");

    // Extract command
    char *executable = token;

    // Because there is an LF at the end of the executable and I'm bored
    size_t len = strlen(executable);
    if (executable[len - 1] == '\n')
    {
        executable[len - 1] = '\0';
    }

    // Extracting arguments
    int arg_count = 0;
    char **args = (char **)malloc(sizeof(char *));
    args[0] = NULL;

    // Initialize Command struct directly in createCommand
    Command *cmd = createCommand(executable, args, arg_count, 0);

    // Variables to handle redirection
    int has_input_redirection = 0;
    char *input_file = NULL;
    int has_output_redirection = 0;
    char *output_file = NULL;
    char *here_document_content = NULL;
    int append_output = 0;

    while (token != NULL)
    {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            // Check for input redirection(<)
            if (strcmp(token, "<") == 0)
            {
                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    has_input_redirection = 1;
                    input_file = strdup(token);
                }
                else
                {
                    fprintf(stderr, "Error: Missing input file after '<'\n");
                    freeCommand(cmd);
                    free(inputCopy);
                    return NULL;
                }
            }
            // Check for output redirection (>)
            else if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
            {
                has_output_redirection = 1;
                output_file = strdup(token);

                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    output_file = strdup(token);
                }
                else
                {
                    fprintf(stderr, "Error: Missing output file after '%s'\n", output_file);
                    freeCommand(cmd);
                    free(inputCopy);
                    return NULL;
                }
            }
            // Check for append output redirection (>>)
            else if (strcmp(token, ">>") == 0) {
                has_output_redirection = 1;
                append_output = 1;

                token = strtok(NULL, " ");
                if (token != NULL) {
                    output_file = strdup(token);
                } else {
                    fprintf(stderr, "Error: Missing output file after '>>'\n");
                    freeCommand(cmd);
                    free(inputCopy);
                    return NULL;
                }
            }
            // Check for here document (<<)
            else if (strcmp(token, "<<") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    here_document_content = strdup(token);
                } else {
                    fprintf(stderr, "Error: Missing content for here document (<<)\n");
                    freeCommand(cmd);
                    free(inputCopy);
                    return NULL;
                }
            }
            // Regular arguments
            else
            {
                arg_count++;
                args = realloc(args, (arg_count + 1) * sizeof(char *));
                args[arg_count - 1] = strdup(token);
                args[arg_count] = NULL;
            }
        }
    }

    // Assign redirection information to the Command struct
    cmd->has_input_redirection = has_input_redirection;
    cmd->input_file = input_file;
    cmd->has_output_redirection = has_output_redirection;
    cmd->output_file = output_file;
    cmd->here_document_content = here_document_content;
    cmd->append_output = append_output;

    // Debug print the Command structure
    printf("Command Structure: \n");
    printf("Executable: %s\n", cmd->executable);
    for (int i = 0; i < arg_count; i++)
    {
        printf("Argument %d: %s\n", i + 1, args[i]);
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