
#include "ordenaciones.h"


void ordenarArrQSort(double *nums, int n){
    // Declara y ordena un array con qsort
    //double nums[] = {4.55, -1.56, 9.9, 56.1, -0.89};
    //int n = sizeof(nums) / sizeof(double);

    imprimir(nums, n);
    qsort(nums, n, sizeof(double), criterio);
    imprimir(nums, n);
}
