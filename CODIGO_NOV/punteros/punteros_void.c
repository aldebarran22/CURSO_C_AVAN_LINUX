// Punteros genéricos: void *


#include <stdio.h>


void casting(void* ptr){
    int* num = (int *) ptr;

    printf("\nnum: %d", *num);
}

int main(){
    int x = 100;
    casting(&x);
    puts("");

    double y = 123.55;
    casting(&y);
    puts("");

    return 0;
}

