// Punteros constantes y punteros a constantes:

#include <stdio.h>


void punterosCtes(){
    int x = 100;
    int y = 200;

    printf("\nx = %d", x);

    // Fijamos el puntero:
    int * const p = &x;

    // Podemos modificar el contenido
    *p = 101;
    printf("\nx = %d", x);

    // No podemos cambiar la direccion del puntero:
    // p = &y; // ERROR!
}


int main(){
    punterosCtes();
    puts("");
    return 0;
}