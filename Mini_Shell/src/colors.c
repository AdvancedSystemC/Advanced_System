#include <stdio.h>
#include "colors.h"

void green() {
    printf("\033[0;32m");
}

void blue() {
    printf("\033[0;34m");
}

void reset () {
  printf("\033[0m");
}