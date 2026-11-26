#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*

gcc -std=c11 -g -O0 deadlock.c -o deadlock -pthread
valgrind --tool=helgrind ./deadlock

*/

typedef void *(*rutina_t)(void *);

static pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
 
static pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

static void *thread_a(void *argument)
{
    // Para evitar warnings si no se utiliza el parametro!
    (void)argument;

    pthread_mutex_lock(&mutex_a);
    puts("Hilo A: mutex_a adquirido");

    sleep(1);
    puts("\nHilo A intenta coger el mutex_b");

    pthread_mutex_lock(&mutex_b);
    puts("Hilo A: mutex_b adquirido");

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);

    return NULL;
}

static void *thread_b(void *argument)
{
    (void)argument;

    pthread_mutex_lock(&mutex_b);
    puts("Hilo B: mutex_b adquirido");

    sleep(1);
    puts("\nHilo B intenta coger el mutex_a");

    pthread_mutex_lock(&mutex_a);
    puts("Hilo B: mutex_a adquirido");

    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);

    return NULL;
}

int main(){
    pthread_t h1, h2;

    if (pthread_create(&h1, 0, (rutina_t) thread_a, 0)){
        puts("Error al crear el hilo1");
        pthread_mutex_destroy(&mutex_a);
        pthread_mutex_destroy(&mutex_b);

        return EXIT_FAILURE;
    }

    if (pthread_create(&h2, 0, (rutina_t) thread_b, 0)){
        puts("Error al crear el hilo2");
        pthread_mutex_destroy(&mutex_a);
        pthread_mutex_destroy(&mutex_b);

        return EXIT_FAILURE;
    }

    pthread_join(h1, 0);
    pthread_join(h2, 0);

    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);

    return EXIT_SUCCESS;
}
