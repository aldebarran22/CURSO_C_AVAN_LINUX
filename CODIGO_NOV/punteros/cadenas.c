// Cadenas:

#include <stdio.h>
#include <string.h>


void cadenas(){
    // Mal el destino no tiene espacio reservado: array de  chars, o malloc
    char *ptr;
    const char* mensaje = "hola que tal";

    strcpy(ptr, mensaje);
    puts(ptr);
}

void cadenas_ok(){
    char ptr[50]; // reservando en pila
    const char* mensaje = "hola que tal";

    strcpy(ptr, mensaje);
    puts(ptr);
}


void restarPunteros(){
    const char* mensaje = "hola que tal";

    char *ptr = strchr(mensaje, 't');
    char *ptr2 = strchr(mensaje, 'a');

    printf("\nresta: %p - %p = %d", ptr2, ptr, ptr2-ptr);
    puts("");
}

int main(){
    
    cadenas_ok();
    return 0;
}