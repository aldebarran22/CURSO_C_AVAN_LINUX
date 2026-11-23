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

void punterosACtes(){
    const char *mensaje = "hola que tal";

    //mensaje[0] = 'X'; // No puedo modificar el contenido
    const int x = 100;
    const int y = 200;

    const int* ptr = &x;
    printf("\n dir ptr: %p", ptr);

    // Puedo cambiar el puntero:
    ptr = &y;
    printf("\n dir ptr: %p", ptr);

    // Pero no podemos cambiar el contenido:
    // *ptr = 300; // ERROR!
}


int main(){
    punterosCtes();    
    puts("");

    punterosACtes();    
    puts("");

    return 0;
}