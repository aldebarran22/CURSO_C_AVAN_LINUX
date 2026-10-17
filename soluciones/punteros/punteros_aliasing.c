
#include <stdio.h>

void aliasing(){
    // Todo bien, tipos compatibles
    int x = 5;
    int* a = &x;
    int* b = &x;
    *b = 10;
    printf("Aliasing tipos compatibles: %d\n", *a);
}

void aliasing_incompatibles(){
    // OJO tipos incompatibles:
    // Comportamiento indefinido
    int x = 5;
    int *a = &x;
    float *b = (float *)&x;

    *b = 10.0f;
    printf("Aliasing tipos incompatibles: %d\n", *a);
}

void aliasing_restrict(int *restrict a, int *restrict b){
    // Con restrict le decimos al compilador que
    // no lo vamos a utilizar con las mismas direcciones
    // Violamos la promesa:
    *a = 5;
    *b = 10;

    printf("Restrict: %d\n", *a);
}

int main(){

    aliasing();
    puts("");
    aliasing_incompatibles();
    puts("");

    int x;
    aliasing_restrict(&x, &x);
    puts("");
    return 0;
}