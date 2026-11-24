// Macros con parámetros:

#include <stdio.h>

#define MENOR(A,B) ((A<B)?(A):(B))


int main(){
    int a = 1, b = 5;

    printf("\nmenor int: %d", MENOR(a,b));
    printf("\nmenor float: %f", MENOR(12.7f,6.8f));
    printf("\nmenor double: %f", MENOR(12.7,6.8));
    printf("\nmenor long: %ld", MENOR(12L,7L));

    puts("");
    return 0;
}