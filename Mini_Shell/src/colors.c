#include <stdio.h>
#include "colors.h"


/**
 * @brief set the text color to green.
 */
void green() {
    printf("\033[0;32m");
}


/**
 * @brief set the text color to blue.
 */
void blue() {
    printf("\033[0;34m");
}


/**
 * @brief Reset the text color to the default.
 */
void reset () {
  printf("\033[0m");
}