#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_THREADS 10

//Tabla con los identificadores de los threads
pthread_t tabla_thr[MAX_THREADS];
//Definimos un mutex en caso querramos serializar recursos //compartidos (en nuestro caso la consola) o para evitar race //conditions
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int ultimothread;

//definimos la estructura de datos que usaremos para darle argumentos a la función del thread

typedef struct {
	int id; //identificador del thread
	char* cadena; //mensaje al thread
} thr_param_t;
thr_param_t param[MAX_THREADS];

void* funcion_thr(void *pointer) {
	//creamos un mutex ya que la consola es un recurso compartido y queremos bloquearla para que no la use otro thread
	pthread_mutex_lock(&mutex1);
	thr_param_t* parametro;
	parametro = (thr_param_t *) pointer;
	printf("%s leido por thread nro: %d\n", parametro->cadena, parametro->id);

	//para saber cual fue el ultimo thread en ejecutarse
	ultimothread++;

	//liberamos el mutex ya que el recurso de la consola y la variable ultimothread ya no estan libres
	pthread_mutex_unlock(&mutex1);
	//una vez terminado, devolvemos el valor
	pthread_exit(&(parametro->id));
}

int main(void) {
	int i, id;

	//necesitamos definir un puntero tonto para dar argumentos y recibir datos al-hacia la función del thread

	void *dummypointer;

	ultimothread = 0;

	//Iniciando creacion de los threads
	puts("Iniciando creacion de los threads:\n");
	for (i = 0; i < MAX_THREADS; i++) {
		param[i].cadena = strdup("Hola Mundo!");
		param[i].id = i;
		dummypointer = (void *) &param[i];
		pthread_create(&tabla_thr[i], NULL, funcion_thr, dummypointer);
	}
	puts("Los threads fueron creados. Esperando que terminen.\n");

	//Recopilamos datos devueltos por los threads con //la función pthread_join
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_join(tabla_thr[i], &dummypointer);
		id = *((int *) dummypointer);
		printf("El thread %d devolvio %d\n", i, id);
	};

	
	return 0;
};

