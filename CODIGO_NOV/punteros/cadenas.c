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

int contarRepeticiones(const char* cadena, int letra){
    char *resul;
    int ini=0;
    int pos;
    int cont=0;

    resul = strchr(cadena+ini, letra);
    while (resul != NULL){
        cont++;
        pos = resul - cadena;
        ini = pos + 1;
        resul = strchr(cadena+ini, letra);
    }
    return cont;
}


void restarPunteros(){
    const char* mensaje = "hola que tal";

    char *ptr = strchr(mensaje, 't');
    char *ptr2 = strchr(mensaje, 'a');

    printf("\nresta: %p - %p = %ld", ptr2, ptr, ptr2-ptr);
    puts("");

    printf("%p ", strchr(mensaje, 'z'));
}

int main(){
    
    //cadenas_ok();
    restarPunteros();
    char s[200] = "hola que tal";
    printf("\nRepeticiones de a: %d", contarRepeticiones(s, 'a'));
    return 0;
}