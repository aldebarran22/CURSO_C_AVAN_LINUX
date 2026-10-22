#include "datos.h"
#include "productor.h"


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