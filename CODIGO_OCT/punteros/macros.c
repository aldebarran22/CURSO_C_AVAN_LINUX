
// macros, problemas con parametros:

#include <stdio.h>

#define MENOR(A,B) ((A<B)?(A):(B))

int main(){
    int x = 9, y = 10;

    printf("\nMenor 6,8: %d", MENOR(6,8));
    printf("\nMenor 6.99,-8.4: %f", MENOR(6.99,-8.4));
    printf("\nMenor a,A: %c", MENOR('a','A'));

    printf("\nx = %d, y = %d", x, y);
    int m = MENOR(x++,y++);
    printf("\nMenor: %d", m);
    printf("\nx = %d, y = %d", x, y);

    puts("");
    return 0;
}