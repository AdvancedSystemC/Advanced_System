#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

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
 * @return 0 on successful execution.
 */
int main()
{
    initAliasManager();

    if (isatty(STDIN_FILENO))
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
                // Remove the newline character from the input
                userInput[strcspn(userInput, "\n")] = 0;
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
    else
    {
        // shell is running in non-interactive mode
    }

    return 0;
}