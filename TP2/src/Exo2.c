#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd[2];

    if (argc != 2)
    {
        printf("Erreur : nombre de paramètres incorrect.\n");
        return 1;
    }

    if (pipe(fd) == -1)
    {
        perror("Erreur lors de la création du tube");
        return 1;
    }

    pid = fork();

    if (pid == -1)
    {
        printf("Erreur : échec de la création du processus fils.\n");
        return 1;
    }

    char filename[] = "/tmp/proc-exerciseXXXXXX";

    if (pid == 0)
    {
        // Processus fils
        printf("PID du processus fils : %d\n", getpid());
        close(1);
        close(fd[0]); // Fermer l'extrémité de lecture du tube

        int fd_temp = mkstemp(filename);
        if (fd_temp == -1)
        {
            perror("Erreur lors de la création du fichier temporaire");
            exit(1);
        }

        printf("Numéro du descripteur de fichier ouvert : %d\n", fd_temp);
        dprintf(fd[1], "%s", filename); // Écrire le nom de fichier dans le tube
        close(fd[1]); // Fermer l'extrémité d'écriture du tube

        if (execlp(argv[1], argv[1], NULL) == -1)
        {
            perror("Erreur lors de l'exécution du programme");
            exit(1);
        }
    }
    else
    {
        // Processus père
        printf("PID du processus père : %d\n", getpid());

        close(fd[1]); // Fermer l'extrémité d'écriture du tube

        char filename_received[256];
        int n = read(fd[0], filename_received, 256); // Lire le nom de fichier à partir du tube
        if (n == -1)
        {
            perror("Erreur lors de la lecture du tube");
            return 1;
        }
        filename_received[n] = '\0';

        printf("Nom de fichier reçu : %s\n", filename_received);

        wait(NULL); // Attendre que le processus fils se termine

        // Ouvrir le fichier temporaire en lecture
        FILE *file = fopen(filename_received, "r");
        if (file == NULL)
        {
            perror("Erreur lors de l'ouverture du fichier temporaire");
            return 1;
        }

        // Lire et afficher le contenu du fichier
        char c;
        while ((c = fgetc(file)) != EOF)
        {
            putchar(c);
        }

        fclose(file);

        printf("That's All Folks !\n");
    }

    return 0;
}
