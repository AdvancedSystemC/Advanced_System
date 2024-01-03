#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "../include/history.h"
#include "../include/display.h"
#include "../include/commandManager.h"
#include "../include/inputParser.h"
#include "../include/typedef.h"
#include "../include/alias.h"

/**
 * @brief Get the current working directory.
 * @return The current working directory as a string.
 */
char *getDirectory()
{
    char *cwd = (char *)malloc(sizeof(char) * 1024);
    if (getcwd(cwd, 1024) != NULL)
    {
        return cwd;
    }
    else
    {
        perror("Cannot get current directory");
        return NULL;
    }
}

/**
 * @brief The main function of the shell program.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on successful execution.
 */
int main(int argc, char *argv[])
{
    initAliasManager();

    if (argc > 1 && strcmp(argv[1], "-c") == 0)
    {
        // shell is running in batch mode
        if (argc > 2)
        {
            char *batchCommand = argv[2];
            Command *userCommand = parseInput(batchCommand);
            int exitStatus = executeCommand(userCommand);
            freeCommand(userCommand);
        }
        else
        {
            printf("No batch command provided.\n");
        }
    }
    else if (isatty(STDIN_FILENO))
    {
        // shell is running in interactive mode
        char *username = getenv("USER");
        char hostname[1024];
        gethostname(hostname, sizeof(hostname));
        const char *userInput[1024];
        const char *delimiter = " \t\n="; // Define the token delimiter

        display_welcome();

        while (1)
        {
            display_prompt(username, hostname, getDirectory());

            if (fgets(userInput, sizeof(userInput), stdin) != NULL)
            {
                char *newline = strchr(userInput, '\n');
                if (newline)
                    *newline = 0;
            }

            // Input Treatment
            Command *userCommand = parseInput(userInput);

            // TO DO need to be done in the Parseafter & treatment
            addCommandToHistory(userInput);

            // Command Execution
            int exitStatus = executeCommand(userCommand);

            // Clean up
            freeCommand(userCommand);
        }
    }

    return 0;
}