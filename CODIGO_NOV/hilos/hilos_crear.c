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

    for (int i = 0 ; i <  *n ; i++){
        printf("\nid = %ld, mensaje: %d", pthread_self(), (i+1));
        sleep(1);
    }

    puts("\nTermina el hilo mensajes ...");
}

void *aleatorios(void *p){
    // Devuelve el resultado (la suma) a traves de un campo de la estructura!
    datos_t *d = p;
    int num;

    for (int i = 0 ; i < d->n ; i++){
        num = rand() % 100;
        d->resul += num;

        printf("\nAleatorio: %d = %d", (i+1), num);
        sleep(rand() % 3);
    }

    puts("\nTermina el hilo aleatorio ...");
}

void *aleatorios2(void *p){
    // Devuelve  el resultado en return
    int *n = p;
    int suma = 0, num;
    int *resul = NULL;

    for (int i = 0 ; i <  *n ; i++){
        num = rand() % 100;
        suma += num;

        printf("\nAleatorio: %d = %d", (i+1), num);
        sleep(rand() % 3);
    }

    // Ya tenemos el resultado en suma:
    resul = malloc(sizeof(int));
    *resul = suma;
    return (void *) resul;
}

int main(){
    pthread_t idM;
    pthread_t idA;
    pthread_t idA2;

    int num = 8;
    datos_t d = {10, 0};
    int *resultado; // Para recuperar el resultado de join

    // Crear los hilos:
    if (pthread_create(&idM, 0, mensajes, &num)){
        puts("\nError  al crear el hilo Mensajes");
        return EXIT_FAILURE;
    }


    if (pthread_create(&idA, 0, aleatorios, &d)){
        puts("\nError  al crear el hilo Aleatorios");
        return EXIT_FAILURE;
    }

    if (pthread_create(&idA2, 0, aleatorios2, &num)){
        puts("\nError  al crear el hilo Aleatorios2");
        return EXIT_FAILURE;
    }

    // Esperamos a que terminen los hilos:
    pthread_join(idM, 0);    
    pthread_join(idA, 0);
    pthread_join(idA2, (void **) &resultado);

    printf("\nResultado aleatorios: %d", d.resul);
    printf("\nResultado aleatorios2: %d", *resultado);

    free(resultado);
    resultado = NULL;

    puts("\nTermina main ...");
    return EXIT_SUCCESS;
}