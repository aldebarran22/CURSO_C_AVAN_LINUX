#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
Activar una prueba, compilar, ejecutar Valgrind y 
después pasar a la siguiente:

COMPILACION:
gcc -std=c11 -Wall -Wextra -Wpedantic \
-g3 -O0 -fno-omit-frame-pointer errores_memoria2.c \
-o errores_memoria2

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
./errores_memoria2

*/

// Error 1: liberar dos veces el mismo bloque!
/*
static void test_doble_free(void){
    int *numeros = malloc(10 * sizeof(int));

    if  (numeros  == NULL){
        perror("malloc");
        return;
    }

    free(numeros);
    free(numeros);
}
*/

// Error 2: acceso a una  posicion de un array despues de liberar:
/*
static void test_acceso_despues_free(void){
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    *numeros = 100;
    free(numeros);

    printf("\nPosicion 0: %d", *numeros);
}
*/

// Error 3: Devolver la direccion de una variable local:
/*
static int *getPunteroLocal(void){
    int numero = 100;

    // Devolver la dir de numero: ojo es local!
    return &numero;
}

static void test_dir_variable_local(void){
    int *ptr = getPunteroLocal();

    printf("\nContenido de ptr: %d", *ptr);
}
*/

// Error 4: overflow / underflow, escribir fuera de los limites de un bloque:
/*
static void test_overflow_heap(void){
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    printf("\nAcceso al elemento 12: %d", numeros[11]);
    free(numeros);
}*/

static void test_underflow_heap(void){
    // Lo detecta valgrind como na lectura invalida del tamaño de sizeof(tipo)
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    printf("\nAcceso al elemento -1: %d", numeros[-1]);
    free(numeros);
}

// No lo detecta valgrind
/*
static void test_overflow_stack(void){
    int numeros[] = {1,2,3,4,5};
   
    printf("\nAcceso al elemento 12: %d", numeros[11]);   
}
*/


int main(){
    //test_doble_free();
    //test_acceso_despues_free();
    //test_dir_variable_local();

    //test_overflow_heap();
    //test_overflow_stack();
    test_underflow_heap();

    puts("");
    return 0;
}