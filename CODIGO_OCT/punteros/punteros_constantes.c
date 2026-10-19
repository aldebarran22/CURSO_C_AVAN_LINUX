
// Punteros constantes y punteros a  constantes!

#include <stdio.h>
#include <string.h>


void punterosCtes(){
    int x = 100;
    int y = 200;

    int * const p = &x;
    *p = 101;

    puts("\nPunteros cte");
    printf("\nx = %d", x);

    //p = &y; // NO podemos cambiar la dir. del puntero, pero si la variable
}

void punterosACtes(){
    const char *path = "fichero.txt";
    const int x = 20;
    const int y = 30;

    //path[0] = 'F'; // NO podemos modificar el contenido, es cte.

    puts("\nPunteros a cte");

    const int *ptr = &x;
    printf("\n*ptr = %d", *ptr);

    ptr = &y;
    printf("\n*ptr = %d", *ptr);
}

void punterosCtesACtes(){
    const int x = 25;
    const int y = 25;

    // Es cte el puntero y el contenido del puntero:
    const int* const p = &x;

    // *p =  0; // Error cambiar el contenido
    // p = &y; // Error cambiar el puntero.
}

void leerFichero(const char *path){
    //path[0] = 'x';
}
 

int main(){
    punterosCtes();
    punterosACtes();
    punterosCtesACtes();
    leerFichero("/home/fichero");

    puts("");
    return 0;
}