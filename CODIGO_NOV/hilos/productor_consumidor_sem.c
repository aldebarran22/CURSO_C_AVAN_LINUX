// Implementacion del productor / consumidor con semaforos

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_PROD 2
#define NUM_CON 2
#define TAM_BUFFER 6
#define NUM_MUESTRAS 30

typedef struct {
    char buffer[TAM_BUFFER+1];
    int index_p; // Indice del P    
    int index_c; // Indice del C

    pthread_mutex_t mutex;
    sem_t sem_huecos;
    sem_t sem_items;

    int num_muestras_p; // Num. de muestras a generar
    int num_muestras_c; // Num. de muestras a procesar
} data_t;

void *productor(void *p){

}

void *consumidor(void *p){

}

int main(){
    if (NUM_MUESTRAS % NUM_PROD != 0 || NUM_MUESTRAS % NUM_CON != 0){
        puts("Trabajo no equilibrado");
        return EXIT_FAILURE;
    }

    pthread_t productores[NUM_PROD];
    pthread_t consumidores[NUM_CON];

    // Inicializar la estructura:
    data_t d;
    d.index_c = d.index_p = 0;
    d.num_muestras_p = NUM_MUESTRAS / NUM_PROD;
    d.num_muestras_c = NUM_MUESTRAS / NUM_CON;    
    memset(d.buffer, '-', TAM_BUFFER);

    // Crear el mutex y los semaforos:
    pthread_mutex_init(&d.mutex, 0);
    sem_init(&d.sem_items, 0, 0);
    sem_init(&d.sem_huecos, 0, TAM_BUFFER);

    for (int i = 0 ; i < NUM_PROD ; i++){
        pthread_create(&productores[i], 0, productor, &d);
    }

    for (int i = 0 ; i < NUM_CON ; i++){
        pthread_create(&consumidores[i], 0, consumidor, &d);
    }

    for (int i = 0 ; i < NUM_PROD ; i++){
        pthread_join(productores[i], 0);
    }

    for (int i = 0 ; i < NUM_CON ; i++){
        pthread_join(consumidores[i], 0);
    }

    pthread_mutex_destroy(&d.mutex);
    sem_destroy(&d.sem_huecos);
    sem_destroy(&d.sem_items);

    return EXIT_SUCCESS;
}
