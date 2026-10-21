#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *worker(void *p){
    puts("Durmiendo 10 sg");
    //sleep(10);
    for (int i = 0 ; i < 10 ; i++){
        if (i == 5){
            puts("\nEl hilo hace exit");
            pthread_exit(NULL);
        }
    }
    puts("\nTermina: worker");
}

int main(void){
    pthread_t thread;
    pthread_create(&thread, NULL, worker, NULL);
    //pthread_detach(thread);

    puts("\nmain va a ejecutar exit()");
     // Termina el hilo que lo ejecuta
    puts("\nFin main");
}