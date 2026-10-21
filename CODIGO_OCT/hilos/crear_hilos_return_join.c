// Crear dos hilos y esperar a que terminen
// Uno de los hilos devuelve un valor a traves
// de join.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void *mensajes(void *p){
    int *n = (int *)p;

    printf("\nID hilo mensajes: %ld", pthread_self());
    for (int i = 0 ; i < *n ; i++){
        printf("\nMensaje %d", (i+1));
        sleep(rand()%2);
    }
    puts("\nmensajes: termina");
   
}

void *aleatorios(void *p){
    int *resul = NULL;
    int *n = (int *)p;
    int suma = 0;
    int num;


    printf("\nID hilo aleatorios: %ld", pthread_self());
    for (int i = 0 ; i < *n ; i++){
        num = rand()%100;
        suma += num;

        printf("\nNumero %d = %d", (i+1), num);
        sleep(rand()%3);
    }
    printf("\naleatorios: termina con suma = %d", suma);
 
    // Reservar para devolver un resultado:
    resul = (int *) malloc(sizeof(int));
    *resul = suma;
    return (void *) resul;

}


int main(){
    // Un id por cada hilo
    pthread_t idMensaje;
    pthread_t idNums;
    int *resultado;

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
    pthread_join(idNums, (void **) &resultado);

    printf("\nResultado en main: %d\n", *resultado);
    free(resultado);
    resultado = NULL;

    puts("\nThread main termina ...");
    exit(EXIT_SUCCESS);
}
