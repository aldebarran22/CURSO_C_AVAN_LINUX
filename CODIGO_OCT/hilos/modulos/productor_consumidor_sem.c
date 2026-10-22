// Productor - Consumidor para varios.
// Implementacion con semaforos y mutex

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>





void *productor(void *p){
    data_t *d = (data_t *) p;

    for (int i = 0 ; i < d->muestras_p ; i++){
        // Generar un item:
        char c = 65 + (rand() % 26);
        printf("\nP%ld genera: %c", pthread_self(), c);

        // Comprobar si hay huecos:
        sem_wait(&d->sem_huecos);

        // Colocar el item en exclusion mutua:
        pthread_mutex_lock(&d->mutex);

        // Colocar el item:
        d->buffer[d->index_p] = c;

        // Mover el indice:
        d->index_p = (d->index_p + 1) % TAM_BUFFER;

        // Imprimir el buffer:
        printf("\nBUFFER: %s\n", d->buffer);

        // liberar  el mutex:
        pthread_mutex_unlock(&d->mutex);

        // Avisar de que se ha colocado un item en el buffer:
        sem_post(&d->sem_items);

        sleep(rand()%5);

    }
}

void *consumidor(void *p){
    data_t *d = (data_t *) p;
    char c;

    for (int i = 0 ; i < d->muestras_c ; i++){
        
        // Comprobar si hay items:
        sem_wait(&d->sem_items);

        // Obtener el item en exclusion mutua:
        pthread_mutex_lock(&d->mutex);

        // Extraer el item:
        c = d->buffer[d->index_c];

        printf("\nC%ld obtiene %c", pthread_self(), c);

        d->buffer[d->index_c] = '-';

        // Mover el indice:
        d->index_c = (d->index_c + 1) % TAM_BUFFER;


        // Imprimir el buffer:
        printf("\nBUFFER: %s\n", d->buffer);

        // liberar  el mutex:
        pthread_mutex_unlock(&d->mutex);

        // Avisar de que se ha quitado un item en el buffer:
        sem_post(&d->sem_huecos);

        sleep(rand()%5);
    }
}

int main(){
    if (NUM_MUESTRAS % N_PROD != 0 || NUM_MUESTRAS % N_CON != 0){
        puts("Valores incorrectos ");
        return 1;
    }

    pthread_t productores[N_PROD];
    pthread_t consumidores[N_CON];

    // Inicializar la estructura:
    data_t d;
    d.index_c = 0;
    d.index_p = 0;
    d.muestras_c = NUM_MUESTRAS / N_CON;
    d.muestras_p = NUM_MUESTRAS / N_PROD;
    strcpy(d.buffer, "------");
    pthread_mutex_init(&d.mutex, 0);
    sem_init(&d.sem_huecos, 0, TAM_BUFFER);
    sem_init(&d.sem_items, 0, 0);

    // Lanzar los hilos:
    for (int i = 0 ; i < N_PROD ; i++){
        pthread_create(&productores[i], 0, productor, &d);
    }

    for (int i = 0 ; i < N_CON ; i++){
        pthread_create(&consumidores[i], 0, consumidor, &d);
    }

    // JOIN:
    for (int i = 0 ; i < N_PROD ; i++){
        pthread_join(productores[i], 0);
    }

    for (int i = 0 ; i < N_CON ; i++){
        pthread_join(consumidores[i], 0);
    }

    // Liberar recursos:
    pthread_mutex_destroy(&d.mutex);
    sem_destroy(&d.sem_huecos);
    sem_destroy(&d.sem_items);

    return 0;
}