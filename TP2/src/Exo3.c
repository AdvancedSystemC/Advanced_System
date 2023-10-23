#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pipefd[2];
    pid_t pid;

    //Création du pipe
    if(pipe(pipefd) == -1){
        perror("Erreur lors de la création du pipe");
        exit(EXIT_FAILURE);
    }

    //Creation du processys fils
    pid = fork();

    if(pid < 0){
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        //Processus Fils

        // Fermeture de l'extrémité d'écriture du pipe
        close(pipefd[0]);

        // Redirection de l'entrée standard vers l'extrémité de lecture du pipe
        dup2(pipefd[1], STDIN_FILENO);

        // Exécution de la commande "ps eaux"
        execlp("ps", "ps", "eaux", NULL);

        // En cas d'erreur lors de l'exécution de la commande
        perror("Erreur lors de l'exécution de la commande ps");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Processus parent
        // Fermeture de l'extrémité d'écriture du pipe
        close(pipefd[1]);

        // Redirection de l'entrée standard vers l'extrémité de lecture du pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Exécution de la commande "grep "^root " > /dev/null"
        execlp("grep", "grep", "^root", NULL);

        // En cas d'erreur lors de l'exécution de la commande
        perror("Erreur lors de l'exécution de la commande grep");
        exit(EXIT_FAILURE);
    }

    return 0;

}