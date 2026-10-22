// Productor - Consumidor para varios.
// Implementacion con semaforos y mutex

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N_PROD 1
#define N_CON 1
#define TAM_BUFFER 6
#define NUM_MUESTRAS 30


typedef struct {
    char buffer[TAM_BUFFER+1];
    int index_p;
    int index_c;
    pthread_mutex_t mutex;
    sem_t sem_items;
    sem_t sem_huecos;
} data_t;

