
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>


#define ELEMENTOS_BUFER 16
#define TOTAL_DATOS 20

void *productor(void *);
void *consumidor(void *);

// Estructura de datos compartida por los hilos
struct {
  int bufer[ELEMENTOS_BUFER]; // matriz de datos
  int ind_p, ind_c; // índices del productor y del consumidor
  pthread_mutex_t exmut;      // exclusión mutua
  sem_t sem_vacios; // semáforo contador de elementos vacíos
  sem_t sem_llenos; // semáforo contador de elementos llenos
} st_bufer;

int main(){
  pthread_t hilo_p, hilo_c; // hilos productor y consumidor

  // Iniciar la estructura st_bufer
  st_bufer.ind_p = st_bufer.ind_c = 0;
  pthread_mutex_init(&st_bufer.exmut, 0);
  sem_init(&st_bufer.sem_vacios, 0, ELEMENTOS_BUFER);
  sem_init(&st_bufer.sem_llenos, 0, 0);

  // Crear los hilos
  pthread_create(&hilo_p, 0, productor, 0);
  pthread_create(&hilo_c, 0, consumidor, 0);

  // Esperar a que los hilos trabajadores terminen
  pthread_join(hilo_p, 0);
  pthread_join(hilo_c, 0);

}

// Productor
void *productor(void *arg){
  int i, a;

  for(i = 0; i < TOTAL_DATOS; i++){
    sem_wait(&st_bufer.sem_vacios); // decrementar el contador de vacíos
    pthread_mutex_lock(&st_bufer.exmut); // echar el cerrojo

    // Bufer circular: añadir dato
    a = rand() % 100;
    st_bufer.bufer[st_bufer.ind_p] = a;
    st_bufer.ind_p = (st_bufer.ind_p + 1) % (ELEMENTOS_BUFER);
    printf("produce buffer  --> %06d\n", a);

    pthread_mutex_unlock(&st_bufer.exmut); // quitar el cerrojo
    sem_post(&st_bufer.sem_llenos); // incrementar el contador de llenos
  }
  return 0;
}

// Consumidor
void *consumidor(void *arg){
  int i, a;

  for(i = 0; i < TOTAL_DATOS; i++){
    sem_wait(&st_bufer.sem_llenos); // decrementar el contador de llenos
    pthread_mutex_lock(&st_bufer.exmut); // echar el cerrojo

    // Bufer circular: recoger elemento
    a = st_bufer.bufer[st_bufer.ind_c];
    st_bufer.ind_c = (st_bufer.ind_c + 1) % (ELEMENTOS_BUFER);
    printf("consume buffer  --> %06d\n", a);

    pthread_mutex_unlock(&st_bufer.exmut); // quitar el cerrojo
    sem_post(&st_bufer.sem_vacios); // incrementar el contador de vacíos
  }
  return 0;
}

