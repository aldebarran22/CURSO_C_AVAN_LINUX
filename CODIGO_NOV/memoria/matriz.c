#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5

typedef struct {
    int filas;
    int cols;
    int **m;
} matriz_t;


inicializar();

imprimir();

liberar();


int main(){
    srand(time(NULL));

    matriz_t matriz = {N, N*2, NULL};

    inicializar();
    imprimir();
    liberar();

    puts("");
    return 0;
}