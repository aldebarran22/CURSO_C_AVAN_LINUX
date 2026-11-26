// Hilos que modifican dos contadores: uno protegido con un mutex y otro no (RC)

/*

gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=thread \
    hilos_mutex_RC.c -o hilos_mutex_RC -pthread

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_ITER 10000000

int contador=0;
int contador_mtx = 0;

pthread_mutex_t mutex;


void *sumar(void *p){

    (void)p;
    for (int i = 0 ; i < N_ITER ; i++){
        contador++;
    }
    return NULL;
}

void *restar(void *p){
    (void)p;
    for (int i = 0 ; i < N_ITER ; i++){
        contador--;
    }
    return NULL;
}

void *sumar2(void *p){

    (void)p;
    for (int i = 0 ; i < N_ITER ; i++){
        pthread_mutex_lock(&mutex);
        contador_mtx++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *restar2(void *p){
    (void)p;
    for (int i = 0 ; i < N_ITER ; i++){
        pthread_mutex_lock(&mutex);
        contador_mtx--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_t id1;
    pthread_t id2;
    pthread_t id3;
    pthread_t id4;

    pthread_mutex_init(&mutex, 0);
   
    // Crear los hilos:
    if (pthread_create(&id1, 0, sumar, 0)){
        puts("\nError  al crear el hilo 1");
        return EXIT_FAILURE;
    }

     // Crear los hilos:
    if (pthread_create(&id2, 0, restar, 0)){
        puts("\nError  al crear el hilo 2");
        return EXIT_FAILURE;
    }

     if (pthread_create(&id3, 0, sumar2, 0)){
        puts("\nError  al crear el hilo 3");
        return EXIT_FAILURE;
    }

     // Crear los hilos:
    if (pthread_create(&id4, 0, restar2, 0)){
        puts("\nError  al crear el hilo 4");
        return EXIT_FAILURE;
    }

    pthread_join(id1, 0);
    pthread_join(id2, 0);
    pthread_join(id3, 0);
    pthread_join(id4, 0);

    printf("\ncontador: %d", contador);
    printf("\ncontador_mtx: %d", contador_mtx);
    puts("");

    pthread_mutex_destroy(&mutex);

    return 0;
}