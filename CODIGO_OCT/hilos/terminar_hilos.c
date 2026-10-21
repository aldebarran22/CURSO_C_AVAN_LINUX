#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *worker(void *p){
    puts("Durmiendo 10 sg");
    sleep(10);
}

int main(void){
    pthread_t thread;
    pthread_create(&thread, NULL, worker, NULL);
    pthread_detach(thread);

    puts("\nmain va a ejecutar exit()");
    pthread_exit(NULL); // Termina el hilo que lo ejecuta
    
}