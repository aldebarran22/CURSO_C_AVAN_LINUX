// Pruebas con Hilos:
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void *tarea1(void *);
void *tarea2(void *);

void *tarea1_lock(void *);
void *tarea2_lock(void *);

int contador=0;
int contador_lock=0;

pthread_mutex_t mutex;

int main(){
	// Definir los identificadores de los hilos:
	srand(time(NULL));
	
	pthread_t id1, id2, id1_lock, id2_lock;
	
	// Crear el mutex, ojo antes de crear los threads:
	pthread_mutex_init(&mutex, 0);
	
	// Crear los hilos:
	if (!pthread_create(&id1, 0, tarea1, NULL))
		printf("\nHilo1 creado: id = %ld", id1);
		
	if (!pthread_create(&id2, 0, tarea2, NULL))
		printf("\nHilo2 creado: id = %ld", id2);
		
	if (!pthread_create(&id1_lock, 0, tarea1_lock, NULL))
		printf("\nHilo1_lock creado: id = %ld", id1_lock);
		
	if (!pthread_create(&id2_lock, 0, tarea2_lock, NULL))
		printf("\nHilo2_lock creado: id = %ld", id2_lock);
	
	pthread_join(id1,0);
	pthread_join(id2,0);
	
	pthread_join(id1_lock,0);
	pthread_join(id2_lock,0);
	
	// Liberar el mutex:
	pthread_mutex_destroy(&mutex);
	
	puts("\n\nHilo principal termina");
	
	printf("\nEl valor del contador: %d", contador);
	printf("\nEl valor del contador con lock: %d\n\n", contador_lock);
	
}

void *tarea1(void *params){
	long i;
	
	for (i = 0 ; i < 10000000 ; i++){
		contador++;
	}
}

void *tarea2(void *params){
	long i;
	
	for (i = 0 ; i < 10000000 ; i++){
		contador--;
	}
}

void *tarea1_lock(void *params){
	long i;
	
	for (i = 0 ; i < 10000000 ; i++){
		pthread_mutex_lock(&mutex);
		contador++;
		pthread_mutex_unlock(&mutex);
	}
}

void *tarea2_lock(void *params){
	long i;
	
	for (i = 0 ; i < 10000000 ; i++){
		pthread_mutex_lock(&mutex);
		contador--;
		pthread_mutex_unlock(&mutex);
	}
}
