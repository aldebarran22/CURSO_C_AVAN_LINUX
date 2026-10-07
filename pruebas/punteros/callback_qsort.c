// Uso  de punteros a funcion para hacer una llamada a qsort

// Usamos un callback para establecer el criterio de ordenacion

#include <stdio.h>
#include <stdlib.h>


int cmp_int(const void*a, const void* b){
    // Castea el contenido de los dos parametros
    int ia = *(const int*) a;
    int ib = *(const int*) b;

    // Devuelve 1, 0, -1
    return (ia > ib) - (ia < ib);
}

void imprimir(int *p, int n){
    for (int i = 0 ; i  < n ; i++){
        printf("%d ", p[i]);
    }
    puts("");
}


int main(){
    int v[] = {3,5,6,23,21,6,9,9,0};
    int  n = sizeof(v) / sizeof(int);

    imprimir(v, n);
    qsort(v, n, sizeof(v[0]), cmp_int);
    imprimir(v, n);

    puts("");
    return 0;
}