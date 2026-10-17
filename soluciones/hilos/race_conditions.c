// Para compilar:
/*
gcc -std=c11 -Wall -Wextra -Wpedantic \
    race_conditions.c -o race_conditions -pthread
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_COUNT 4
#define INCREMENTS 1000000

static int counter = 0;

static void *worker(void *argument)
{
    (void)argument;

    for (int i = 0; i < INCREMENTS; ++i) {
        counter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i) {
        int error = pthread_create(
            &threads[i],
            NULL,
            worker,
            NULL
        );

        if (error != 0) {
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Esperado: %d\n", THREAD_COUNT * INCREMENTS);
    printf("Obtenido: %d\n", counter);

    return EXIT_SUCCESS;
}
