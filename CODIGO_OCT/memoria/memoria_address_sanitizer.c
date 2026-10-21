// Pruebas de errores en memoria con
// AddressSanitizer

/*
gcc -std=c11 \
-Wall -Wextra -Wpedantic \
-g -O1 \
-fsanitize=address \
-fno-omit-frame-pointer \
memoria_address_sanitizer.c \
-o memoria_address_sanitizer

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
static void test_doble_free(void){
    int *numeros = malloc(10 * sizeof(int));

    if  (numeros  == NULL){
        perror("malloc");
        return;
    }

    free(numeros);
    free(numeros);
}*/



static void test_overflow_heap(void){
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    for (int i = 0 ; i < 10 ; i++){
        numeros[i] = i+1;
    }

    printf("\nAcceso al elemento 12: %d", numeros[11]);
    free(numeros);
}


static void test_underflow_heap(void){
    // Lo detecta valgrind como una lectura invalida del tamaño de sizeof(tipo)
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    printf("\nAcceso al elemento -1: %d", numeros[-1]);
    free(numeros);
}

// No lo detecta valgrind

static void test_overflow_stack(void){
    int numeros[] = {1,2,3,4,5};
   
    printf("\nAcceso al elemento 12: %d", numeros[11]);   
}



static void test_laguna_memoria(void){
    // Lo detecta valgrind como: definitely lost: 40 bytes in 1 blocks

    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    // NO hacemos free!
}


int main(){
    //test_doble_free();
    test_laguna_memoria();
    test_overflow_heap();
    test_overflow_stack();
    test_underflow_heap();
    return 0;
}
