#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    // Création d'un tube pour la communication entre ps et grep
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Création d'un processus fils
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Code du fils
        // Fermeture du côté lecture du tube
        close(pipe_fd[0]);

        // Redirection de la sortie standard (stdout) vers le tube
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Exécution de la commande "ps eaux" avec execlp
        execlp("ps", "ps", "eaux", NULL);

        // En cas d'erreur
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Code du parent
        // Fermeture du côté écriture du tube
        close(pipe_fd[1]);

        // Redirection de l'entrée standard (stdin) depuis le tube
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        // Création d'un nouveau processus pour exécuter le grep
        pid_t grep_pid = fork();

        if (grep_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (grep_pid == 0) { // Code du fils (grep)
            // Redirection de la sortie standard (stdout) vers /dev/null
            int dev_null = open("/dev/null", O_WRONLY);
            dup2(dev_null, STDOUT_FILENO);
            close(dev_null);

            // Exécution de la commande "grep "^root ""
            execlp("grep", "grep", "^root", NULL);

            // En cas d'erreur
            perror("execlp");
            exit(EXIT_FAILURE);
        } else { // Code du parent
            // Attente de la fin du fils (grep)
            waitpid(grep_pid, NULL, 0);

            // Affichage du message "root est connecté"
            write(STDOUT_FILENO, "root est connecté\n", 18);
        }

        // Attente de la fin du fils (ps)
        waitpid(child_pid, NULL, 0);
    }

    return 0;
}
