
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int execute_grep(char **toks) {
    if (toks[1] == NULL) {
        printf("grep: missing argument\n");
        return 1;
    }
    if (toks[2] == NULL) {
        printf("grep: missing file operand\n");
        return 1;
    }
    if (toks[3] != NULL) {
        printf("grep: too many arguments\n");
        return 1;
    }
    char *args[] = {"grep", toks[1], toks[2], NULL};
    execvp("grep", args);
    perror("grep");
    return 1;
}