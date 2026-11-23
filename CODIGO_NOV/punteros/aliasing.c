// Aliasing: dos punteros apuntan a la misma dir. de memoria

#include <stdio.h>

void aliasing(){
    int x = 5;

    int *a = &x;
    int *b = &x;

    *b = 10;
    printf("\n*a = %d", *a);
}

void aliasing_incompatible(){
    // Mezclar distintos tipos en el aliasing, puede dar lugar
    // a comportamiento indefinido del compilador: UB (unknow Behaviour)
    int x = 5;

    int *a = &x;
    float *b = &x;

    *b = 10.0f;
    printf("\nTipos incompatibles: *a = %d", *a);
}

void aliasing_restrict(int * restrict a, int * restrict b){
    // Con restrict le decimos al compilador que no lo 
    // vamos a utilizar con las mismas direcciones de memoria
    // Afecta a la optimizacion.    
    *a = 5;
    *b = 10;

    printf("\na: %d, b: %d", *a, *b);
}


int main(){
    //aliasing();
    //aliasing_incompatible();

    int a = 10, b = 20;
    aliasing_restrict(&a, &a);

    puts("");
    return 0;
}