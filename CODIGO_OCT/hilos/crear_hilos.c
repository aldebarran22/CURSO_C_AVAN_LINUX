// Crear dos hilos y esperar a que terminen

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void *mensajes(void *p){

}

void *aleatorios(void *p){

}


int main(){
    // Un id por cada hilo
    pthread_t idMensaje;
    pthread_t idNums;

    int nMen = 10;
    int nNum = 7;

    // Crear  los hilos:
    if (pthread_create(&idMensaje, 0, mensajes, &nMen)){
        puts("\nError al crear el hilo 1 ...");
        exit(EXIT_FAILURE);

    }

    if (pthread_create(&idNums, 0, aleatorios, &nNum)){
        puts("\nError al crear el hilo 2 ...");
        exit(EXIT_FAILURE);

    }

    // Esperar a que terminen los hilos:
    pthread_join(idMensaje, 0);
    pthread_join(idNums, 0);


    puts("Thread main termina ...");
    exit(EXIT_SUCCESS);
}
