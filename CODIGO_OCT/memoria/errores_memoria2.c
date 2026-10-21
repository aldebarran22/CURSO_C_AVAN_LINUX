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

/*
static void test_underflow_heap(void){
    // Lo detecta valgrind como una lectura invalida del tamaño de sizeof(tipo)
    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    printf("\nAcceso al elemento -1: %d", numeros[-1]);
    free(numeros);
}*/

// No lo detecta valgrind
/*
static void test_overflow_stack(void){
    int numeros[] = {1,2,3,4,5};
   
    printf("\nAcceso al elemento 12: %d", numeros[11]);   
}
*/

// Error 5: Laguna de memoria (memory leak):
/*
static void test_laguna_memoria(void){
    // Lo detecta valgrind como: definitely lost: 40 bytes in 1 blocks

    int *numeros = calloc(10, sizeof(int));

    if (numeros  == NULL){
        perror("calloc");
        return;
    }

    // NO hacemos free!
}*/

// Error 6: Perder el bloque original al ampliar el tamaño con realloc:
/*
static void test_fallo_realloc(void){
    // Valgrind: avisa con: Warning: set address range perms: large range
    // Reservar un bloque inicial con malloc:
    int *numeros = malloc(10 * sizeof(int));

    if (numeros  == NULL){
        perror("malloc");
        return;
    }

    // Intentar coger un bloque muy grande con realloc, pero soble el
    // mismo puntero:
    numeros = realloc(numeros, 1000000000ULL * sizeof(*numeros));
    if (numeros == NULL){
        puts("ha fallado realloc");
        return;
    }

    free(numeros);
}
*/

// Error 7: Reservar para copiar un string, pero la cadena origen es mas grande
/*
static void test_string_overflow(void){    

    char *ptr = malloc(5 * sizeof(char));

    if (ptr == NULL){
        perror("malloc");
        return;
    }
    
    strcpy(ptr, "hola que tal estas");
    puts(ptr);

    free(ptr);
    ptr  = NULL;
}
*/

// Error 8: intentar liberar memoria que no fue reservada:
/*
static void test_free_no_reservada(void){
    int x = 100;

    int *ptr = &x;

    // Ojo, la variable se ha creando en el STACK no esta en el HEAP
    free(ptr);
}*/

// Error 9: Acceso a una posicion no inicializada:
static void test_posicion_no_inicializada(void){

    int *numeros = malloc(5 * sizeof(int));
    if (numeros == NULL){
        perror("malloc");
        return;
    }

    numeros[0] = 999;

    printf("\nAcceso a una posicion no inicializada: %d", numeros[1]);

    free(numeros);
    numeros = NULL;
}

int main(){
    //test_doble_free();
    //test_acceso_despues_free();
    //test_dir_variable_local();

    //test_overflow_heap();
    //test_overflow_stack();
    //test_underflow_heap();

    //test_laguna_memoria();
    //test_fallo_realloc();

    //test_string_overflow();
    //test_free_no_reservada();

    test_posicion_no_inicializada();

    puts("");
    return 0;
}