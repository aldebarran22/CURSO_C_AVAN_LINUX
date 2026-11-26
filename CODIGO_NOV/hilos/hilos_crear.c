// Crear dos hilos con distintas tareas:

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int n;
    int resul;
} datos_t;


void *mensajes(void *p){
    int *n = p;
}

void *aleatorios(void *p){
    datos_t *d = p;


}

int main(){
    pthread_t idM;
    pthread_t idA;

    int num = 8;
    datos_t d = {10, 0};

    // Crear los hilos:
    if (pthread_create(&idM, 0, mensajes, &num)){
        puts("\nError  al crear el hilo Mensajes");
        return EXIT_FAILURE;
    }

    if (pthread_create(&idA, 0, aleatorios, &d)){
        puts("\nError  al crear el hilo Aleatorios");
        return EXIT_FAILURE;
    }

    // Esperamos a que terminen los hilos:
    pthread_join(idM, 0);
    pthread_join(idA, 0);

    return EXIT_SUCCESS;
}