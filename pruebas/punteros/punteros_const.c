// Punteros constantes:

#include  <stdio.h>


void funcion_param_const(const char* nombre){
    // No podemos modificar el parametro, es de solo lectura
    //nombre[0] = 'A';
} 


void punterosAConstantes(){
    const int x = 25;
    const int y = 50;

    // Puntero a la cte. x:
    const int* p1 = &x;

    // Los datos son ctes. pero  el puntero no.
    // El puntero que se puede cambiar a otra cte.
    p1 = &y;


    // No podemos modificar el contenido  de p1:
    //*p1 = 15; // Error

}

void punterosConstantes(){
    char otra_cadena[20] = {"hola"};
    int x = 100;
    int* const p1 = &x;

    // p1 es un puntero cte., pero  *p1 es una variable,
    // Se puede cambiar el contenido  del puntero,
    // EL puntero NO puede apuntar a otra direccion.
    char  nombre[20] = {"Luis"};
    char* const ptr = nombre;
    *ptr = 'c';

    //nombre = otra_cadena;
}

int main(){
    punterosAConstantes();
    punterosConstantes();

    return 0;
}