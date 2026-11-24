#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

inicializar();  // Crear el array e inicializar
imprimir();
liberar(); // libera la memoria

int main(){
    int *ptr = NULL;

    srand(time(NULL));

    ptr = (int *) malloc(sizeof(int) * N);
    for (int i = 0 ; i < N ; i++){
        ptr[i] = rand() % 100;
    }

    for (int i = 0 ; i < N ; i++){
        printf("%d ", ptr[i]);
    }
    puts("");

    free(ptr);
    ptr = NULL;

    return 0;
}
