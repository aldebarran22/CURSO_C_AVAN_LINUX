// Aliasing: dos punteros apuntan a la misma dir. de memoria

#include <stdio.h>

void aliasing(){
    int x = 5;

    int *a = &x;
    int *b = &x;

    *b = 10;
    printf("\n*a = %d", *a);
}


int main(){
    aliasing();

    puts("");
    return 0;
}