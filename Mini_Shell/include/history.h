/**
 * @file history.h
 * @brief This file contains function declarations for command history related operations
*/
#ifndef HISTORY_H
#define HISTORY_H

/**
 * @brief Add a command to the history.txt file
 * @param command The command to be added to the file
*/
void addCommandToHistory(const char* command);

/**
 * @brief display the history //TODO Put it in the display.h under a generic read text function
*/
void printCommandHistory();

#endif // HISTORY_H
