
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


#define MAX 100

// Tipo que define los parámetros de un hilo
typedef struct {
	int id;
	int cuenta;
	char str[MAX];
} pthread_param_t;


int random_(int n) {
	static int primera_vez = 1;
	if (primera_vez) {
		srand(time(0));
		primera_vez = 0;
	}
	return (rand() % n) + 1; // valor entre 1 y n
}

// Función que ejecutan los hilos secundarios
void *tomarMuestra(pthread_param_t *p) {
	p->cuenta = random_(15);
	for (int i = 0; i < p->cuenta; i++)
		printf("%s\n", p->str);
	return &p->id;
}

void resultados(pthread_param_t param[], int n) {
	int i, total = 0;
	for (i = 0; i < n; i++) {
		printf("%s: %d\n", param[i].str, param[i].cuenta);
		total += param[i].cuenta;
	}
	printf( "Total: %d\n", total);
}

void error(int codigo, char *str) {
	printf("%d: %s", codigo, str);
	exit(1);
}

typedef void *(*ptrfnthr)(void *);

// Hilo primario (hilo principal)
int main() {
	const int NHILOS = 3;
	int i, cod, *estado;
	char aux[MAX];

	// Identificadores de los hilos
	pthread_t hilo[NHILOS];

	// Parámetros de los hilos
	pthread_param_t param[NHILOS];

	for (i = 0; i < NHILOS; i++) {
		// Iniciar la estructura i con los argumentos para el hilo i
		param[i].id = i;
		param[i].cuenta = 0;
		strcpy(param[i].str, "Tomando muestra de tipo ");
		sprintf(aux, " %c", 'A' + i);
		strcat(param[i].str, aux);
	}

	// Crear los n hilos e iniciar su ejecución
	for (i = 0; i < NHILOS; i++) {
		cod = pthread_create(&hilo[i], 0, (ptrfnthr) tomarMuestra,(void *) &param[i]);
		if (cod)
			error(cod, "pthread_create");
	}

	// Esperar a que los hilos secundarios terminen
	for (i = 0; i < NHILOS; i++) {
		cod = pthread_join(hilo[i], (void **) &estado);
		if (cod)
			error(cod, "pthread_join");

		// Verificar cómo finaliza el hilo i y liberar sus recursos
		if (*estado != i) {
			printf("el hilo %d ha terminado anormalmente\n", i);
			exit(1);
		}
	}
	// Mostrar resultados
	resultados(param, NHILOS);


}

