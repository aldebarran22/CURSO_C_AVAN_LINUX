#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t mutex_a =
    PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex_b =
    PTHREAD_MUTEX_INITIALIZER;

static void *thread_a(void *argument)
{
    (void)argument;

    pthread_mutex_lock(&mutex_a);
    puts("Hilo A: mutex_a adquirido");

    sleep(1);

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

    pthread_mutex_lock(&mutex_a);
    puts("Hilo B: mutex_a adquirido");

    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);

    return NULL;
}
