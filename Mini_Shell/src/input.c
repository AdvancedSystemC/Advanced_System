#include <stdio.h>
#include <string.h>
#include "../include/input.h"

void read_input(char* input, int input_size) {
    if (fgets(input, input_size, stdin) != NULL) {
        // Remove the newline character from the input
        input[strcspn(input, "\n")] = 0;
    }
}

void process_input(const char* input, const char* delimiter, char* toks[]) {
    int counter = 0;
    char* token = strtok((char*)input, delimiter);

    while (token != NULL) {
        toks[counter] = strdup(token);
        token = strtok(NULL, delimiter);
        counter++;
    }

    toks[counter] = NULL; // Terminate the toks array with a NULL
}