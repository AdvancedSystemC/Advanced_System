#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define STDOUT 1
#define STDERR 2

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    int fd;
    pid_t pid;

    printf("Message: %s\n", argv[1]);

    pid = fork();

    if (pid < 0){
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } 
    else if (pid == 0){
        //Child process
        printf("Child PID: %d\n", getpid());

        // Ferme le descripteur 1 (STDOUT)
        close(STDERR);

        // Ouvre en création et écriture le fichier temporaire /tmp/proc-exercice
        fd = mkstemp("/tmp/proc-exercice");

        // Exécute le programme passé en argument
        execvp(argv[1], &argv[1]);

        //Affiche le numéro du descripteur du fichier ouvert
        printf("File descriptor: %d\n", fd);

        // Exécute le programme passé en argument
        execvp(argv[1], &argv[1]);

        // En cas d'erreur lors de l'exécution du programme
        fprintf(stderr, "Failed to execute program\n");
        return 1;

    }
    else{
        //Processus parent
        printf("Parent PID: %d\n" , getpid());
        wait(NULL);
        printf("That's All Folks!\n");
    }

    return 0;
}
