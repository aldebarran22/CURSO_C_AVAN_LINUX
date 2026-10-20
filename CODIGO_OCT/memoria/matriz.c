// Matriz dinamica:

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

void imprimir(matriz_t m){

    for (int  i = 0 ; i < m.filas ; i++){

        for (int j = 0 ; j < m.cols ; j++){
            printf("%02g\t", m.ptr[i][j]);
        }
        puts("");
    }
}

/*
void liberar(matriz_t *m){

    // Liberar cada fila:
    for (int  i = 0 ; i < m->filas ; i++){
        free(m->ptr[i]);
        m->ptr[i] = NULL;
    }

    // Liberar el puntero exterior:
    free(m->ptr);
    m->ptr = NULL;

    // Poner a cero los contadores:
    m->filas = 0;
    m->cols = 0;
}*/

void liberar(matriz_t *m){

    // Liberar el puntero exterior:
    free(m->ptr);
    m->ptr = NULL;

    // Liberar cada fila:
    for (int  i = 0 ; i < m->filas ; i++){
        free(m->ptr[i]);
        m->ptr[i] = NULL;
    }

 
    // Poner a cero los contadores:
    m->filas = 0;
    m->cols = 0;
}


int main(){

    /* OJO NO HEMOS RESERVADO EN DESTINO, NO HAY SITIO PARA COPIAR LA CADENA!
    char *s;
    char nombre[] = {"Gema"};
    strcpy(s, nombre);
    */
   

    int m2[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};

    matriz_t m = {0, 0, NULL};

    srand(time(NULL));

    inicializar2(&m, 8, 5);
    imprimir(m);
    liberar(&m);

    puts("");
    return 0;
}