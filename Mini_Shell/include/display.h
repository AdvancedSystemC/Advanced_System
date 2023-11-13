/**
 * @file display.h
 * @brief This file contains basic display operations.
 */
#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @brief display welcome message
*/
void display_welcome();

/**
 * @brief display basic prompt
 * @param username The name of the current user
 * @param hostname The name of the host machine
 * @param directory The path to the current directory
*/
void display_prompt(const char* username, const char* hostname , const char* directory);

#endif // DISPLAY_H
