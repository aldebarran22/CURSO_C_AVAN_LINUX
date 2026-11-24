#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

int *inicializar(size_t n){
    // Crear el array e inicializar
    int* ptr = (int *) malloc(sizeof(int) * n);

    for (int i = 0 ; i < n ; i++){
        ptr[i] = rand() % 100;
    }
    return ptr;
}  


void inicializar_v2(int **ptr, int n){
    // Crear el array e inicializar
    *ptr = (int *) malloc(sizeof(int) * n);

    for (int i = 0 ; i < n ; i++){
        (*ptr)[i] = rand() % 100;
    }
}


void inicializar_v3(int **aux, int n){
    int *ptr;

    // Crear el array e inicializar
    ptr = (int *) malloc(sizeof(int) * n);

    for (int i = 0 ; i < n ; i++){
        ptr[i] = rand() % 100;
    }
    *aux = ptr;
}


void imprimir(int *ptr, int n){
    for (int i = 0 ; i < n ; i++){
        printf("%d ", ptr[i]);
    }
    puts("");
}


liberar(); // libera la memoria

int main(){
    int *ptr = NULL;

    srand(time(NULL));

    //ptr = inicializar(N);
    //inicializar_v2(&ptr, N);
    inicializar_v3(&ptr, N);

    imprimir(ptr, N);

    free(ptr);
    ptr = NULL;

    return 0;
}
