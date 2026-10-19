

// Punteros a funcion, pasar funciones como parametros, funciones que devuelven 
// funciones (punteros a funcion)

#include <stdio.h>
#include <stdlib.h>


int suma(int a, int b){
    // Prototipo: int suma(int, int);
    return a+b;
}


void definirPtrFuncion(){
    printf("\nFuncion suma: %p", suma);

    // Definir un ptr a funcion a suma:
    int (*psuma)(int, int);

    // Asignarlo:
    psuma = suma;

    // Ejecutar a través del ptr:
    printf("\n psuma: %d", psuma(2,5));
}


void imprimir(double *ptr, int n){

    printf("\n\nIMPRIMIR: ptr ocupa %d bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %g - %g", i, *(ptr+i), ptr+i);
    }
}

int criterio(const void *p1, const void *p2){
    // 0 iguales, -1 *p1 < *p2, +1 *p1 > *p2
    double d1 = *(double *)p1;
    double d2 = *(double *)p2;

    if (d1 == d2){
        return 0;

    } else if (d1 < d2){
        return -1;
        
    } else if (d1 > d2){
        return 1;
    }
}

void ordenarArray_qsort(){
    double numeros[] = {5.7, 8.88, -9.9, 0.0, 123.6};

    int n = sizeof(numeros) / sizeof(double);
    imprimir(numeros, n);

    // el array, cuantos elems, bytes de cada elem, funcion criterio.
    qsort(numeros, n, sizeof(double), criterio);
    imprimir(numeros, n);
}


int main(){
    //definirPtrFuncion();
    ordenarArray_qsort();

    puts("");
    return 0;
}