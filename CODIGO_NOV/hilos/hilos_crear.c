// Crear dos hilos con distintas tareas:

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int n;
    int resul;
} datos_t;


void *mensajes(void *p){

}

void *aleatorios(void *p){

}

int main(){
    pthread_t idM;
    pthread_t idA;

    // Crear los hilos:

}