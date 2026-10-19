

#include <stdio.h>


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


int main(){
    definirPtrFuncion();

    puts("");
    return 0;
}