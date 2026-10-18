#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MUESTRAS 10
// elementos de la matriz de datos

// Estructura de datos compartida por los hilos
struct p {
	int muestras[MAX_MUESTRAS]; // matriz de datos
	int ind; // índice del primer elemento vacío
	pthread_mutex_t mutex_adquirir; // exclusión mutua
} st_bufer;

int random_(int n) {
	static int primera_vez = 0;
	if (primera_vez) {
		srand(time(0));
		primera_vez = 0;
	}
	return (rand() % n) + 1; // valor entre 1 y n
}

void error(int codigo, char *str) {
	printf("codigo : %d, %s\n", codigo, str);
	exit(1);
}

// Hilo trabajador
void *AdquirirDatos(void *pid) {
	int x = 0, cod, id = *((int *) pid);
	do {
		cod = pthread_mutex_lock(&st_bufer.mutex_adquirir);
		if (cod)
			error(cod, "pthread_mutex_lock");

		if (st_bufer.ind >= MAX_MUESTRAS)
			return pid;
		x = random_(32767); // adquirir un dato
		printf("hilo-%d tomo la muestra: %d\n", id, st_bufer.ind);
		st_bufer.muestras[st_bufer.ind] = x;
		st_bufer.ind++; // incrementar el índice de la matriz

		cod = pthread_mutex_unlock(&st_bufer.mutex_adquirir);
		if (cod)
			error(cod, "pthread_mutex_unlock");
	} while (st_bufer.ind < MAX_MUESTRAS);
	return 0;
}

// Hilo principal
int main() {
	const int NHILOS = 2; // número de hilos
	int i, cod;

	// Iniciar la estructura st_bufer
	st_bufer.ind = 0;
	for (i = 0 ; i < MAX_MUESTRAS ; i++)
		st_bufer.muestras[i] = 0;

	cod = pthread_mutex_init(&st_bufer.mutex_adquirir, 0);
	if (cod)
		error(cod, "pthread_mutex_init");

	// Crear los n hilos e iniciar su ejecución
	pthread_t hilo[NHILOS]; // identificadores de los hilos
	int id[NHILOS]; // mis identificadores: 0, 1, ...
	for (i = 0; i < NHILOS; i++) {
		id[i] = i;
		cod = pthread_create(&hilo[i], 0, AdquirirDatos, &id[i]);
		if (cod)
			error(cod, "pthread_create");
	}

	// Esperar a que los hilos secundarios terminen
	for (i = 0; i < NHILOS; i++) {
		cod = pthread_join(hilo[i], 0);
		if (cod)
			error(cod, "pthread_join");
	}
}

