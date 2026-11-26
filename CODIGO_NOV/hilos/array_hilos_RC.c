// Definir array de hilos y generar una condicion de carrera

/*
Compilar:
gcc -std=c11 -Wall -Wextra -Wpedantic
-fsanitize=thread
array_hilos_RC.c -o array_hilos_RC

*/


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define THREAD_COUNT 4
#define IT 1000000

// Variable privada al modulo:
static int contador = 0;

void *worker(void *p){
    // Evitamos los warnings por no usar el parámetro
    (void) p;

    for (int i = 0 ; i < IT ; i++){
        contador++;
    }    

    return NULL;
}

int main(){
    pthread_t hilos[THREAD_COUNT];

    for (int i = 0 ; i < THREAD_COUNT ; i++){
        int error = pthread_create(&hilos[i], 0, worker, 0);

        if (error){
            return EXIT_FAILURE;
        }        
    }

    for (int i = 0 ; i < THREAD_COUNT ; i++){
        pthread_join(hilos[i], NULL);
    }

    printf("\nValor esperado: %d", THREAD_COUNT * IT);
    printf("\nValor  obtenido: %d", contador);

    puts("");
    return EXIT_SUCCESS;
}