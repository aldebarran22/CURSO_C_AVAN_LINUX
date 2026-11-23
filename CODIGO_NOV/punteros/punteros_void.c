// Punteros genéricos: void *
// En los threads: void *funcion(void *);


#include <stdio.h>


void casting(void* ptr){
    double* num = (double *) ptr;

    printf("\nnum: %f", *num);
    printf("\nnum: %f",*(double *) ptr);
}

void recorrerBytes(const void* buffer, int n){
    // Ordenacion de bytes es little endian: primero el byte menos significativo
    // El casting se hace implitamente:
    const unsigned char *p = buffer;


    for (int i = n-1 ; i>= 0 ; i--){
        printf("%02X ", p[i]);
    }
    puts("");
}

void referencia(void *p){
    // Primero hay que hacer el casting:
    //printf("\nReferencia: %d", *p);
    printf("\nReferencia: %d", *(int*)p);
}

int main(){
    int x = 100;
    casting(&x); // Enviamos un puntero que no es el esperado!
    puts("");

    double y = 123.55;
    casting(&y);    
    puts("");

    int num = 123;
    printf("\nNum: %d %X\n", num, num);
    recorrerBytes(&num, sizeof(num));
    puts("");

    return 0;
}

