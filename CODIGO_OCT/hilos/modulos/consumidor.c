#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "datos.h"
#include "consumidor.h"

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