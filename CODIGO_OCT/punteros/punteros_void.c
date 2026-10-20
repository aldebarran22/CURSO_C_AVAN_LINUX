// Problemas que podemos tener con los punteros void


#include <stdio.h>


void accesoAlContenido(){
    // Cuidado con acceder al contenido de un puntero cuando sea void
    int x = 10;

    // El casting se hace implicitamente
    void *p = &x;

    //*p = 20; // Error, no se puede modificar a traves del puntero void

    // hace el casting int * y luego modifica con el operador *
    *((int *)p) = 20;

    printf("\nx = %d", x);
}


void rutina_mal(void *p){
    // Ciudado con un error de casting a un tipo que no era el original!
    float *ptr = (float *) p;

    printf("\nValor de *ptr: %f", *ptr);
    
}

void recorrerBytes(const void *buf, size_t n){
    // Recorrer en little endian: primero el byte menos significativo
    const unsigned char *p = buf;

    for (int i = n - 1 ; i >= 0 ; i--){
        printf("%02X ", p[i]);
    }
    puts("");
}


int main(){
    accesoAlContenido();
    puts("");

    int x = 100;
    rutina_mal(&x);
    puts("");

    int num = 123;
    recorrerBytes(&num, sizeof(num));
    printf("\nnum: %d %X", num, num);

    return 0;
}