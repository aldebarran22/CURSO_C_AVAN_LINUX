// Punteros genéricos: void *


#include <stdio.h>


void casting(void* ptr){
    double* num = (double *) ptr;

    printf("\nnum: %f", *num);
    printf("\nnum: %f",*(double *) ptr);
}

int main(){
    int x = 100;
    casting(&x); // Enviamos un puntero que no es el esperado!
    puts("");

    double y = 123.55;
    casting(&y);    
    puts("");

    return 0;
}

