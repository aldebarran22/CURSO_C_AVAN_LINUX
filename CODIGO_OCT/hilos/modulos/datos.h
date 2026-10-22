#pragma once

#include <pthread.h>
#include <semaphore.h>

#define N_PROD 2
#define N_CON 2
#define TAM_BUFFER 6
#define NUM_MUESTRAS 20


typedef struct {
    char buffer[TAM_BUFFER+1];
    int index_p;
    int index_c;
    pthread_mutex_t mutex;
    sem_t sem_items;
    sem_t sem_huecos;
    int muestras_p;
    int muestras_c;
} data_t;
