#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Compteur initialisé à 0
int counter = 0;

// Fonction de gestion du signal SIGINT
void handle_sigint(int sig) {
    counter++;
}

// Fonction de gestion du signal SIGTERM
void handle_sigterm(int sig) {
    printf("Fin du programme\n");
    exit(0);
}

int main() {
    // Associer la fonction de gestion du signal SIGINT au signal
    signal(SIGINT, handle_sigint);

    // Associer la fonction de gestion du signal SIGTERM au signal
    signal(SIGTERM, handle_sigterm);

    // Boucle sans fin
    while (1) {
        // Attendre les signaux
    }

    // Afficher le nombre de signaux SIGINT reçus lorsque CTRL+C est pressé
    printf("Nombre de signaux SIGINT reçus : %d\n", counter);

    return 0;
}