#include <stdio.h>
#include "../include/history.h"

FILE* historyFile;

void addCommandToHistory(const char* command) {
    historyFile = fopen("history.txt", "a+"); // Open history file in append mode
    fprintf(historyFile, "%s\n", command);
    fclose(historyFile);
}

void printCommandHistory() {
    char command[100];
    int count = 1;
    historyFile = fopen("history.txt", "r"); // Open history file in read mode

    if (!historyFile) {
        perror("Unable to open history file");
        return;
    }

    while (fgets(command, sizeof(command), historyFile) != NULL) {
        printf("%d. %s", count, command);
        count++;
    }

    fclose(historyFile);
}

int execute_history(char **args)
{
    printCommandHistory();
    return 1;
}