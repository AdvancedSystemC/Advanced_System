#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        int lastDigit = getpid() % 10;
        exit(lastDigit);
    } else {
        // Parent process
        printf("Child PID: %d\n", pid);
        int status;
        wait(&status);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}