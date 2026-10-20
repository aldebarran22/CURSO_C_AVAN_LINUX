// Matriz dinamica:

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int filas;
    int cols;
    double **ptr;
} matriz_t;

matriz_t inicializar(int f, int c){

}

void inicializar2(matriz_t *m, int f, int c){
    m->filas = f;
    m->cols = c;

    // Reservar  el numero de filas
    m->ptr = (double **) malloc(f * sizeof(double *));

    // Reservar el numero de cols.
    for (int  i = 0 ; i < f ; i++){
        m->ptr[i] = (double *) malloc(c * sizeof(double));

        for (int j = 0 ; j < c ; j++){
            m->ptr[i][j] = rand() % 100;
        }
    }
}

imprimir();
liberar();


int main(){
    int m2[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};

    matriz_t m = {0, 0, NULL};

    srand(time(NULL));

    puts("");
    return 0;
}





