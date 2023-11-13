#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE (int)1e8
int tab[SIZE];

// Structure pour stocker les résultats
typedef struct {
    int min;
    int max;
    double min_time;
    double max_time;
    pthread_mutex_t mutex;
} ThreadResults;

// Fonction pour trouver le minimum et maximum dans une plage du tableau
void* findMinMax(void* arg) {
    ThreadResults* results = (ThreadResults*)arg;

    struct timeval start, end;

    // Recherche du minimum
    gettimeofday(&start, NULL);
    int local_min = tab[0];
    for (int i = 1; i < SIZE / 2; ++i) {
        if (tab[i] < local_min) {
            local_min = tab[i];
        }
    }
    gettimeofday(&end, NULL);

    // Verrouillage du mutex avant de mettre à jour les résultats globaux
    pthread_mutex_lock(&results->mutex);
    results->min = local_min;
    results->min_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    pthread_mutex_unlock(&results->mutex);

    // Recherche du maximum
    gettimeofday(&start, NULL);
    int local_max = tab[SIZE / 2];
    for (int i = SIZE / 2 + 1; i < SIZE; ++i) {
        if (tab[i] > local_max) {
            local_max = tab[i];
        }
    }
    gettimeofday(&end, NULL);

    // Verrouillage du mutex avant de mettre à jour les résultats globaux
    pthread_mutex_lock(&results->mutex);
    results->max = local_max;
    results->max_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    pthread_mutex_unlock(&results->mutex);

    pthread_exit(NULL);
}

int main() {
    // Initialisation du tableau avec des valeurs aléatoires
    for (int i = 0; i < SIZE; ++i) {
        tab[i] = rand();
    }

    // Nombre de threads
    int num_threads = 2;

    // Tableau des threads et des résultats
    pthread_t threads[num_threads];
    ThreadResults results;
    results.min = results.max = 0;
    results.min_time = results.max_time = 0.0;
    pthread_mutex_init(&results.mutex, NULL);

    // Création des threads
    for (int i = 0; i < num_threads; ++i) {
        pthread_create(&threads[i], NULL, findMinMax, (void*)&results);
    }

    // Attente de la fin des threads
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Affichage des résultats
    pthread_mutex_lock(&results.mutex);
    printf("Minimum: %d\n", results.min);
    printf("Maximum: %d\n", results.max);
    printf("Temps de recherche du minimum: %f secondes\n", results.min_time);
    printf("Temps de recherche du maximum: %f secondes\n", results.max_time);
    pthread_mutex_unlock(&results.mutex);

    // Destruction du mutex
    pthread_mutex_destroy(&results.mutex);

    return 0;
}