#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Compteur initialisé à 0
static int counter = 0;

// Fonction de gestion du signal SIGINT
void handle_sigint(int sig) {
    counter++;
    printf(" Nombre de SIGINT détectées : %d\n" , counter);
}

// Fonction de gestion du signal SIGTERM
void handle_sigterm(int sig) {
    printf("Signal SIGTERM reçu. Fin du programme.\n");
    exit(0);
}

int main() {

    // Affiche le PID - plus simple pour kill
    printf("PID du processus : %d\n", getpid());

    // Associer la fonction de gestion du signal SIGINT au signal
    signal(SIGINT, handle_sigint);
    // Associer la fonction de gestion du signal SIGTERM au signal
    signal(SIGTERM, handle_sigterm);

    // Boucle sans fin
    while (1) {
        // rien
    }

    return 0;
}