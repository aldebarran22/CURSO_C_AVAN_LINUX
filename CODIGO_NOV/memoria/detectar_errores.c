#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
Activar una prueba, compilar, ejecutar Valgrind y 
después pasar a la siguiente:

COMPILACION:
gcc -std=c11 -Wall -Wextra -Wpedantic \
-g3 -O0 -fno-omit-frame-pointer detectar_errores.c \
-o detectar_errores

parametros:
-std=c11
    Utiliza el estándar C11.

-Wall -Wextra -Wpedantic
    Activa advertencias útiles.

-g3
    Incluye información de depuración detallada:
    fichero, función, número de línea y macros.

-O0
    Desactiva las optimizaciones y facilita relacionar
    las instrucciones con las líneas de C.

-fno-omit-frame-pointer
    Conserva el puntero de marco y facilita obtener
    trazas de llamadas completas.

-o errores_memoria
    Asigna ese nombre al ejecutable.


VALGRIND:
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
./detectar_errores


gcc -std=c11 -g \
-fsanitize=address,undefined \
-fno-omit-frame-pointer \
detectar_errores.c -o detectar_errores

*/

/*
static void test_doble_free(void){
    // Liberar dos veces el mismo puntero:
    int* numeros = malloc(sizeof(int) * 10);

    if (numeros == NULL){
        perror("malloc");
        return;
    }

    free(numeros);
    // numeros = NULL; // Buena práctica, poner los punteros a NULL despues de liberar
    // free sobre un puntero a null no tiene efecto ni error!
    free(numeros);
}*/

/*
static void test_acceso_despues_de_free(void){
    int* numeros = calloc(sizeof(int), 10);

    if (numeros == NULL){
        perror("calloc");
        return;
    }

    free(numeros);
    printf("\nnumeros: %d", *numeros);
}*/

/*
static int* getPunteroLocal(){
    // Se define en la pila, al terminar la funcion se destruye la var.local
    int numero = 0;
    return &numero;
}

static void test_acceso_dir_local(){
    int *ptr = getPunteroLocal();

     printf("\nnumero: %d", *ptr);
}
*/

/*
static void test_overflow_en_heap(){
    // Se detecta con gcc -fsanitize=address
    int* numeros = calloc(sizeof(int), 10);

    if (numeros == NULL){
        perror("calloc");
        return;
    }

    for (int i = 0 ; i < 10 ; i++){
        printf("\ndir: %p - pos: %d", (numeros+i), i);
    }

    printf("\nPosicion 15: %d", numeros[15]);
    free(numeros);
    numeros = NULL;
}*/

/*
static void test_underflow_en_heap(){
    // Se detecta con gcc -fsanitize=address
    int* numeros = calloc(sizeof(int), 10);

    if (numeros == NULL){
        perror("calloc");
        return;
    }

    for (int i = 0 ; i < 10 ; i++){
        printf("\ndir: %p - pos: %d", (void*)(numeros+i), i);
    }

    printf("\nPosicion -1: %d", numeros[-1]);
    free(numeros);
    numeros = NULL;
}
*/

static void test_overflow_en_stack(){
    int numeros[] = {1,2,3,4,5};

    for (int i = 0 ; i < 5 ; i++){
        printf("\ndir: %p - pos: %d", (void*)(numeros+i), i);
    }

    printf("\nPosicion 10: %d", numeros[10]);
}

int main(){

    //test_doble_free();
    //test_acceso_despues_de_free();
    //test_acceso_dir_local();

    //test_overflow_en_heap();
    //test_underflow_en_heap();
    test_overflow_en_stack();


    return 0;
}
