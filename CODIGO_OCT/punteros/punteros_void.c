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


void rutina(void *p){
    float *ptr = (float *) p;

    printf("\nValor de *ptr: %f", *ptr);
    
}


int main(){
    accesoAlContenido();
    puts("");

    int x = 100;
    rutina(&x);
    puts("");

    return 0;
}