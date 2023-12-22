#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include "typedef.h"

/**
 * @brief Create a new Command struct
 * @param executable The command executable
 * @param args Arguments for the command
 * @param arg_count Number of arguments
 * @param has_pipe Indicates if the command is piped
 * @return a new Command struct
 */
Command *createCommand(char *executable, char **args, int arg_count, int has_pipe);

/**
 * @brief Execute a Command
 * @param cmd The command to execute
 * @return the exit status of the command
 */
int executeCommand(Command *cmd);

/**
 * @brief Free memory used by a Command struct
 * @param cmd The command struct to be free
 */
void freeCommand(Command *cmd);

#endif /* COMMAND_MANAGER_H */