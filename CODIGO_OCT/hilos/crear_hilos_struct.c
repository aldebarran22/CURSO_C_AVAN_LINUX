// Crear dos hilos y esperar a que terminen
// Pasar mas de un param con una estructura!

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int n;
    int sg;
} datos_t;

typedef struct {
    int n;
    int suma;
} datos_out_t;



void *mensajes(void *p){
    datos_t *datos = (datos_t *) p;

    printf("\nID hilo mensajes: %ld", pthread_self());
    for (int i = 0 ; i < datos->n ; i++){
        printf("\nMensaje %d", (i+1));
        sleep(rand()%datos->sg);
    }
    puts("\nmensajes: termina");
   
}

void *aleatorios(void *p){
    datos_out_t *datos = (datos_out_t *)p;

    datos->suma = 0;

    printf("\nID hilo aleatorios: %ld", pthread_self());
    for (int i = 0 ; i < datos->n ; i++){
        int num = rand() % 100;
        datos->suma  += num;

        printf("\nNumero %d = %d", (i+1), num);
        sleep(rand()%3);
    }
    puts("\naleatorios: termina");

}


int main(){
    // Un id por cada hilo
    pthread_t idMensaje;
    pthread_t idNums;

    datos_t datos = {10, 3};
    datos_out_t datos2 = {8, 0};

    int nNum = 7;

    // Crear  los hilos:
    if (pthread_create(&idMensaje, 0, mensajes, &datos)){
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

    printf("\nSuma total de los numeros: %d", datos2.suma);

    puts("Thread main termina ...");
    exit(EXIT_SUCCESS);
}
