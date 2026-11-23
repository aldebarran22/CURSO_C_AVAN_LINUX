// Punteros a funcion

#include <stdio.h>

// Declarar el prototipo:
int suma(int, int);


int main(){
    printf("\nDir de suma: %p", suma);

    puts("");
    return 0;
}

int suma(int a, int b){
    return a+b;
}