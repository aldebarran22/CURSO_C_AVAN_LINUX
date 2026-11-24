
#include <stdlib.h>
#include <stdio.h>

#include "ordenaciones.h"

int criterio(const void* p1, const void* p2){
    // 0 = iguales, -1 = p1 < p2, +1 p1 > p2
    // Primero el casting del puntero y despues el contenido
    double n1 = *(double *) p1;
    double n2 = *(double *) p2;

    if (n1 == n2){
        return 0;

    } else if (n1 < n2){
        return -1;

    } else  {
        return 1;
    }
}

void ordenarArrQSort(double *nums, int n){   
    qsort(nums, n, sizeof(double), criterio);    
}


void imprimir(double *v, int n){
    puts("\nArray:");
    for (int i = 0 ; i < n ; i++){
        printf("%g\t", v[i]);
    }
    puts("");
}

