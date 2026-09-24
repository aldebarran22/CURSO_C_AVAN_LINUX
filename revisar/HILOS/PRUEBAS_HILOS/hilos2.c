// Pruebas con Hilos:
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void *tarea1(void *);
void *tarea2(void *);

typedef struct {
	int numMuestras;
	int tMaxEspera;
} TDato;



int main(){
	// Definir los identificadores de los hilos:
	srand(time(NULL));
	
	pthread_t id1, id2;
	TDato dato1 = {12, 1};
	TDato dato2 = {10, 2};
	int *resul1, *resul2;
	
	
	// Crear los hilos:
	if (!pthread_create(&id1, 0, tarea1, &dato1))
		printf("\nHilo1 creado: id = %ld", id1);
		
	if (!pthread_create(&id2, 0, tarea2, &dato2))
		printf("\nHilo2 creado: id = %ld", id2);
	
	pthread_join(id1,(void *) &resul1);
	pthread_join(id2,(void *) &resul2);
	
	puts("\n\nResultado del hilo1");
	printf("Suma: %d", *resul1);
	
	puts("\n\nResultado del hilo2");
	printf("Max: %d", *resul2);
	
	puts("\n\nHilo principal termina ...\n\n");
	free(resul1);
	free(resul2);
	
}

void *tarea1(void *params){
	int i;
	TDato *dato = (TDato *) params;
	int total = 0, muestra;
	int *resul;
	
	for (i = 0 ; i < dato->numMuestras ; i++){
		muestra = rand()%100;
		printf("\n%d) Hilo1 = %d", i, muestra);
		total += muestra;
		sleep(rand()%dato->tMaxEspera);
	}
	resul = (int *) malloc(sizeof(int));
	*resul = total;
	return (void *) resul;
}

void *tarea2(void *params){
	int i;
	TDato *dato = (TDato *) params;
	int max = 0, muestra;
	int *resul;
	
	for (i = 0 ; i < dato->numMuestras ; i++){
		muestra = rand()%200+100;
		printf("\n%d) Hilo2 = %d", i, muestra);
		if (muestra > max) max = muestra;
		
		sleep(rand()%dato->tMaxEspera);
	}
	resul = (int *) malloc(sizeof(int));
	*resul = max;
	return (void *) resul;
}
