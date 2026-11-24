#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5

typedef struct {
    int filas;
    int cols;
    int **m;
} matriz_t;


void inicializar(matriz_t *mat){

    mat->m = malloc(mat->filas * sizeof(int *));

    for (int i = 0 ; i < mat->filas ; i++){
        mat->m[i] = malloc(mat->cols * sizeof(int));

        for (int j = 0 ; j < mat->cols ; j++){
            mat->m[i][j] = rand() % 100;
        }
    }
}

void imprimir(matriz_t mat){

    for (int  i = 0 ; i < mat.filas ; i++){

        for (int j = 0 ; j < mat.cols ; j++){
            printf("%02g\t", mat.m[i][j]);
        }
        puts("");
    }
}

liberar();


int main(){
    srand(time(NULL));

    matriz_t matriz = {N, N*2, NULL};

    inicializar(&matriz);
    imprimir(matriz);
    liberar();

    puts("");
    return 0;
}