
// Error tipico: condicionde carrera
// Varios hilos intentan modificar el mismo recurso
// sin ningun mecanisomo de sincronizacion.

#include <pthread.h>
#include <stdio.h>

/*

gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=thread \
    condicion_carrera.c -o condicion_carrera -pthread

*/


#define N_ITER 10000000

int contador=0;
int contador_mtx = 0;

pthread_mutex_t mutex;


void *sumar(void *p){
    for (int i = 0 ; i < N_ITER ; i++){
        contador++;
    }
}

void *restar(void *p){
    for (int i = 0 ; i < N_ITER ; i++){
        contador--;
    }
}

void *sumar2(void *p){
    for (int i = 0 ; i < N_ITER ; i++){
         pthread_mutex_lock(&mutex);
        contador_mtx++;
        pthread_mutex_unlock(&mutex);
    }
}

void *restar2(void *p){
    for (int i = 0 ; i < N_ITER ; i++){
        pthread_mutex_lock(&mutex);
        contador_mtx--;
        pthread_mutex_unlock(&mutex);
    }
}


int main(){
    pthread_t h1, h2, h3, h4;

    // Crear el mutex: ANTES de crear los hilos!
    pthread_mutex_init(&mutex, 0);

    // Crear los hilos:
    //pthread_create(&h1, 0, sumar, 0);
    //pthread_create(&h2, 0, restar, 0);
    pthread_create(&h3, 0, sumar2, 0);
    pthread_create(&h4, 0, restar2, 0);

    //pthread_join(h1, 0);
    //pthread_join(h2, 0);
    pthread_join(h3, 0);
    pthread_join(h4, 0);

    // Destruir el mutex:
    pthread_mutex_destroy(&mutex);

    //printf("\nContador: %d", contador);
    printf("\nContador_mtx: %d", contador_mtx);
    puts("");
    return 0;
}
