
// Ejemplos de memoria dinamica con arrays

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

int main(){
    int *p;

    srand(time(NULL));

    // Inicializar:
    p = malloc(N * sizeof(int));
    printf("\ndir de  p: %p\n", p);

    for (int i = 0 ; i < N ; i++){
        p[i] = rand() % 100;
    }

    // imprimir:
    for (int i = 0 ; i < N ; i++){
        printf("%d ", p[i]);
    }
    puts("");

    // liberar:
    free(p);
    p = NULL;
    
    free(p);
   


   

    return 0;
}