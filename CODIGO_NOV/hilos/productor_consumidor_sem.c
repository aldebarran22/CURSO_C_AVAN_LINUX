// Implementacion del productor / consumidor con semaforos

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_PROD 1
#define NUM_CON 1
#define TAM_BUFFER 6
#define NUM_MUESTRAS 10

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
    char c;
    data_t *d = p;

    for (int i = 0 ; i < d->num_muestras_p ; i++){
        // generar un char aleatorio
        c = 65 + rand() % 26;
        //printf("\nP: %ld genera %c\n", pthread_self(), c);

        // comprobar si hay huecos
        sem_wait(&d->sem_huecos);

        // comprobar el mutex
        pthread_mutex_lock(&d->mutex);
        printf("\nP: %ld genera %c\n", pthread_self(), c);

        // colocar el char en el buffer, incrementar indice
        d->buffer[d->index_p] = c;
        d->index_p = (d->index_p + 1) % TAM_BUFFER;
        puts(d->buffer);

        // liberar el mutex
        pthread_mutex_unlock(&d->mutex);

        // avisar que hay un item nuevo
        sem_post(&d->sem_items);

        sleep(rand() % 2 + 1);
    }
    return NULL;
}

void *consumidor(void *p){
    char c;
    data_t *d = p;

    for (int i = 0 ; i < d->num_muestras_c ; i++){
    
        // comprobar si hay items
        sem_wait(&d->sem_items);

        // comprobar el mutex
        pthread_mutex_lock(&d->mutex);

        // quitar el char del buffer, incrementar indice
        c = d->buffer[d->index_c];
        d->buffer[d->index_c] = '-';
        d->index_c = (d->index_c + 1) % TAM_BUFFER;

        printf("\nC: %ld procesa %c\n", pthread_self(), c);
        
        // liberar el mutex
        pthread_mutex_unlock(&d->mutex);

        // procesar el item:
        //printf("\nC: %ld procesa %c\n", pthread_self(), c);

        // avisar que hay un hueco nuevo
        sem_post(&d->sem_huecos);

        sleep(rand() % 4 + 1);
    }

    return NULL;
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
