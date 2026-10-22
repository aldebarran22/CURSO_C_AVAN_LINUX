// Productor - Consumidor para varios.
// Implementacion con semaforos y mutex

#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "datos.h"
#include "productor.h"
#include "consumidor.h"


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