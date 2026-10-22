#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *worker(void *p){
    puts("Durmiendo 10 sg");
    sleep(10);    
    puts("\nTermina: worker");
}

int main(void){
    pthread_t thread;
    pthread_create(&thread, NULL, worker, NULL);

    /*
    
    detach() tiene sentido cuando el programa principal 
    seguirá ejecutándose y no necesitas:

    - esperar expresamente al hilo; JOIN
    - obtener su valor de retorno;
    - conocer exactamente cuándo finalizó.

    si main termina un hilo con detach tambien terminara.

    - Probar a poner  esta linea while(1); en main despues de detach
    
    */
    pthread_detach(thread);

    while(1);
        
    puts("\nFin main");
}