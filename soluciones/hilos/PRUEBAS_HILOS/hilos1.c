// Pruebas con Hilos:
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void *tarea1(void *);
void *tarea2(void *);



int main(){
	// Definir los identificadores de los hilos:
	srand(time(NULL));
	
	pthread_t id1, id2;
	
	
	// Crear los hilos:
	if (!pthread_create(&id1, 0, tarea1, NULL))
		printf("\nHilo1 creado: id = %ld", id1);
		
	if (!pthread_create(&id2, 0, tarea2, NULL))
		printf("\nHilo2 creado: id = %ld", id2);
	
	pthread_join(id1,0);
	pthread_join(id2,0);
	
	puts("Hilo principal termina");
	
}

void *tarea1(void *params){
	int i;
	
	for (i = 0 ; i < 5 ; i++){
		printf("\n%d) Hilo1 = %d", i, rand()%100);
		sleep(rand()%2);
	}
}

void *tarea2(void *params){
	int i;
	
	for (i = 0 ; i < 10 ; i++){
		printf("\n%d) Hilo2 = %d", i, rand()%200+100);
		sleep(rand()%3);
	}
}
