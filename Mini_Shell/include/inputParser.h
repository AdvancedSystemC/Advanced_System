#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "typedef.h"

/**
 * @brief Parse user input and create a Command struct
 * @param input The user input string
 * @return a new Command struct based on the input
 */
Command *parseInput(const char *input);

#endif // INPUT_PARSER_H
