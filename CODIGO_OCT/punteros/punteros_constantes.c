
// Punteros constantes y punteros a  constantes!

#include <stdio.h>


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
 

int main(){
    punterosCtes();
    punterosACtes();

    puts("");
    return 0;
}