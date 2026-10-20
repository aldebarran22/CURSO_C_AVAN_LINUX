
// macros, problemas con parametros:

#include <stdio.h>

#define MENOR(A,B) ((A<B)?(A):(B))

int main(){
    int x = 9, y = 10;

    printf("\nMenor 6,8: %d", MENOR(6,8));
    printf("\nMenor 6.99,-8.4: %f", MENOR(6.99,-8.4));
    printf("\nMenor a,A: %c", MENOR('a','A'));

    printf("\nx = %d, y = %d", x, y);

    // OJO CON LAS EXPR. QUE PONEMOS EN LOS PARAMETROS
    int m = MENOR(x++,y++);
    printf("\nMenor: %d", m);
    printf("\nx = %d, y = %d", x, y);

    #warning ojo con las macros parametrizadas
    //#error no podemos continuar  // Ya no compila

    puts("");
    return 0;
}