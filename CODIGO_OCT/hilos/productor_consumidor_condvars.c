// Productor / Consumidor con condvars:
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 8
#define TOTAL_ELEMENTS 20

typedef struct {
    int elements[CAPACITY];
    size_t first;
    size_t count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} queue_t;

static void queue_push(
    queue_t *queue,
    int value)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->count == CAPACITY) {
        pthread_cond_wait(&queue->not_full,
            &queue->mutex
        );
    }

    size_t position =
        (queue->first + queue->count) % CAPACITY;

    queue->elements[position] = value;
    queue->count++;

    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
}

static int queue_pop(queue_t *queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->count == 0) {
        pthread_cond_wait(
            &queue->not_empty,
            &queue->mutex
        );
    }

    int value =
        queue->elements[queue->first];

    queue->first =
        (queue->first + 1) % CAPACITY;

    queue->count--;

    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);

    return value;
}

static void *producer(void *argument)
{
    queue_t *queue = argument;

    for (int value = 1;
         value <= TOTAL_ELEMENTS;
         value++) {
        queue_push(queue, value);

        printf(
            "Productor introduce: %d\n",
            value
        );
    }

    return NULL;
}

static void *consumer(void *argument)
{
    queue_t *queue = argument;

    for (int i = 0;
         i < TOTAL_ELEMENTS;
         i++) {
        int value = queue_pop(queue);

        printf(
            "Consumidor extrae:  %d\n",
            value
        );
    }

    return NULL;
}

int main(void)
{
    queue_t queue = {
        .first = 0,
        .count = 0
    };

    int error = pthread_mutex_init(
        &queue.mutex,
        NULL
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_mutex_init: %s\n",
            strerror(error)
        );

        return EXIT_FAILURE;
    }

    error = pthread_cond_init(
        &queue.not_empty,
        NULL
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_cond_init not_empty: %s\n",
            strerror(error)
        );

        pthread_mutex_destroy(&queue.mutex);
        return EXIT_FAILURE;
    }

    error = pthread_cond_init(
        &queue.not_full,
        NULL
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_cond_init not_full: %s\n",
            strerror(error)
        );

        pthread_cond_destroy(&queue.not_empty);
        pthread_mutex_destroy(&queue.mutex);

        return EXIT_FAILURE;
    }

    pthread_t producer_thread;
    pthread_t consumer_thread;

    error = pthread_create(
        &producer_thread,
        NULL,
        producer,
        &queue
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_create productor: %s\n",
            strerror(error)
        );

        pthread_cond_destroy(&queue.not_full);
        pthread_cond_destroy(&queue.not_empty);
        pthread_mutex_destroy(&queue.mutex);

        return EXIT_FAILURE;
    }

    error = pthread_create(
        &consumer_thread,
        NULL,
        consumer,
        &queue
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_create consumidor: %s\n",
            strerror(error)
        );

        /*
         * El productor podría bloquearse si llena
         * la cola y no existe ningún consumidor.
         */
        pthread_cancel(producer_thread);
        pthread_join(producer_thread, NULL);

        pthread_cond_destroy(&queue.not_full);
        pthread_cond_destroy(&queue.not_empty);
        pthread_mutex_destroy(&queue.mutex);

        return EXIT_FAILURE;
    }

    error = pthread_join(
        producer_thread,
        NULL
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_join productor: %s\n",
            strerror(error)
        );
    }

    error = pthread_join(
        consumer_thread,
        NULL
    );

    if (error != 0) {
        fprintf(
            stderr,
            "pthread_join consumidor: %s\n",
            strerror(error)
        );
    }

    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.not_empty);
    pthread_mutex_destroy(&queue.mutex);

    puts("Fin del programa");

    return EXIT_SUCCESS;
}