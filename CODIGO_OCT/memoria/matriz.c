// Matriz dinamica:

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int filas;
    int cols;
    double **m;
} matriz_t;

inicializar();
imprimir();
liberar();


int main(){
    matriz_t m = {0, 0, NULL};

    srand(time(NULL));

    puts("");
    return 0;
}





