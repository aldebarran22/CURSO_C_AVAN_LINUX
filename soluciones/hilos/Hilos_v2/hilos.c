// hilos.cpp - Un programa C++ visto como un proceso con
//             múltiples hilos
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int random_(int n) {
	static int primera_vez = 1;
	if (primera_vez) {
		srand(time(0));
		primera_vez = 0;
	}
	return (rand() % n) + 1; // valor entre 1 y n
}


void *tomarMuestraTipoA(int *n) {
	*n = random_(10);

	printf("Vamos a tomar %d muestras\n", *n);
	for (int i = 0; i < *n; i++) {
		printf("Tomando muestra de tipo A\n");
	}
	sleep(1);
	return n;
}

void *tomarMuestraTipoB(int *n) {
	*n = random_(15);

	printf("Vamos a tomar %d muestras ...\n", *n);
	for (int i = 0; i < *n; i++) {
		printf("Tomando muestra de tipo B\n");
	}
	sleep(2);
	return n;
}

void resultados(int n, int m) {
	printf("Muestras de tipo A: %d\n", n);
	printf("Muestras de tipo B: %d\n", m);
	printf("Total: %d\n", (n + m));
}

typedef void *(*ptrfnthr)(void *);

int main() {
	int nMuestrasTipoA = 0;
	int nMuestrasTipoB = 0;
	int codigo, *estado;

	// Identificadores de los hilos
	pthread_t hilo1, hilo2;
	// Crear los hilos 1 y 2

	printf("Creamos el hilo 1\n");
	pthread_create(&hilo1, 0, (ptrfnthr) tomarMuestraTipoA, (void *) &nMuestrasTipoA);

	printf("Creamos el hilo 2\n");
	pthread_create(&hilo2, 0, (ptrfnthr) tomarMuestraTipoB, (void *) &nMuestrasTipoB);


	// Esperar a que los hilos terminen
	codigo = pthread_join(hilo1, (void **) &estado );
	codigo = pthread_join(hilo2, (void **) &estado);


	// Mostrar resultados
	resultados(nMuestrasTipoA, nMuestrasTipoB);
}

