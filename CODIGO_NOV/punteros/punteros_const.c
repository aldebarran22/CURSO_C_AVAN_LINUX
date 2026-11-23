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

void punterosCtesACtes(){
    const int x = 25;
    const int y = 25;

    // Es cte el puntero y el contenido del puntero: SOLO LECTURA!
    const int* const p = &x;

    // *p =  0; // Error cambiar el contenido
    // p = &y; // Error cambiar el puntero.
}


int main(){
    punterosCtes();    
    puts("");

    punterosACtes();    
    puts("");

    punterosCtesACtes();    
    puts("");

    return 0;
}