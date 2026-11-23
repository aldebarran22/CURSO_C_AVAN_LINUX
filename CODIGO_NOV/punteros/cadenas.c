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
    char ptr[50];
    const char* mensaje = "hola que tal";

    strcpy(ptr, mensaje);
    puts(ptr);
}


int main(){
    
    cadenas_ok();
    return 0;
}