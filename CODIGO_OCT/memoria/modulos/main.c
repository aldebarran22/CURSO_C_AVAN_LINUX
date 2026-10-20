

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arrays.h"

#define N 10

int main(){
    int *p;

    srand(time(NULL));

    // Llamar a las funciones:
    p = inicializar(N);
    printf("\ndir de  p en main: %p\n", p);

    imprimir(p, N);

    //liberar(p);
    liberar2(&p);
    printf("\ndir de  p despues de liberar: %p\n", p);

    puts("");
    return 0;
}