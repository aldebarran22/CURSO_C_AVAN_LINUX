
// Ejemplos de memoria dinamica con arrays

#include <stdio.h>
#include <stdlib.h>



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
    p = NULL;
}