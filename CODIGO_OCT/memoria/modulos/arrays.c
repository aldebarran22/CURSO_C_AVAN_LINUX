
// Ejemplos de memoria dinamica con arrays

#include <stdio.h>
#include <stdlib.h>

#include "arrays.h"

int *inicializar(int n){
    // Inicializar:
    int *p = malloc(n * sizeof(int));

    printf("\ndir de  p en inicializar: %p\n", p);

    for (int i = 0 ; i < n ; i++){
        p[i] = rand() % 100;
    }

    // Es correcto: es una dir. del HEAP!
    return p;
}

void inicializar2(int **p, int n){
    *p = malloc(n * sizeof(int));

    printf("\ndir de  p en inicializar: %p\n", *p);

    for (int i = 0 ; i < n ; i++){
        (*p)[i] = rand() % 100;
    }

}

void inicializar3(int **p, int n){
    int *aux = malloc(n * sizeof(int));

    printf("\ndir de  p en inicializar: %p\n", aux);
    
    for (int i = 0 ; i < n ; i++){
        aux[i] = rand() % 100;
    }
    *p = aux;
}

void imprimir(int *p, int n){
    // imprimir:
    for (int i = 0 ; i < n ; i++){
        printf("%d ", p[i]);
    }
    puts("");
}

void liberar(int *p){
    // liberar:
    free(p);

    // ¿Realmente cambia el puntero de main?
    // NO CAMBIA EL  PUNTERO DE MAIN!
    p = NULL;
}

void liberar2(int **p){
    free(*p);

    // Si cambia el puntero de main!
    *p = NULL;
}