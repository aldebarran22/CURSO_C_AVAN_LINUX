
// Error tipico: condicionde carrera
// Varios hilos intentan modificar el mismo recurso
// sin ningun mecanisomo de sincronizacion.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N_ITER 10000000

int contador=0;


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

int main(){
    pthread_t h1, h2;

    // Crear los hilos:
    pthread_create(&h1, 0, sumar, 0);
    pthread_create(&h2, 0, restar, 0);

    pthread_join(h1, 0);
    pthread_join(h2, 0);

    printf("\nContador: %d", contador);

    return 0;
}
