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

/*
static void test_overflow_en_stack(){
    // Se detecta con gcc -fsanitize=address
    int numeros[] = {1,2,3,4,5};

    for (int i = 0 ; i < 5 ; i++){
        printf("\ndir: %p - pos: %d", (void*)(numeros+i), i);
    }

    printf("\nPosicion 5: %d", numeros[5]);
}*/


/*
static void test_laguna_memoria(void){
    // valgrind y -fsanitize=address
    int* numeros = calloc(sizeof(int), 10);

    if (numeros == NULL){
        perror("calloc");
        return;
    }
}*/

/*
static void test_fallo_realloc(){
    // valgrind y -fsanitize=address
    int* numeros = malloc(sizeof(int)* 10);

    if (numeros == NULL){
        perror("malloc");
        return;
    }

    // Ampliacion del bloque con realloc, una cantidad muy grande
    // OJO asignar el resultado de realloc a otro puntero auxiliar
    
    //int *aux = realloc(numeros, 100000000000ULL * sizeof(int));
    //if (aux == NULL){
    //    free(numeros);
    //    numeros = NULL;
    //    perror("Fallo con realloc");
    //    return;

    //} else {
    //    numeros = aux;
    //}    
    


    numeros = realloc(numeros, 100000000000ULL * sizeof(int));
    if (numeros == NULL){
        perror("Fallo con realloc");
        return;
    }

    free(numeros);
    numeros = NULL;
}
*/

/*
static void test_overflow_string(void){
    // valgrind (invalid write) y -fsanitize=address
    char *s = malloc(sizeof(char) * 5);

    if (s == NULL){
        perror("Fallo con malloc");
        return;
    }

    strcpy(s, "Hola que tal");
    puts(s);

    free(s);
    s = NULL;
}*/

/*
static void test_free_var_en_stack(){
    int numero = 100;

    int *p = &numero;
    free(p);
}*/

static void test_acceso_no_init(void){
    int* numeros = malloc(sizeof(int)* 10);

    if (numeros == NULL){
        perror("malloc");
        return;
    }

    numeros[0] = 999;

    printf("\nPos: 1 %d", numeros[1]);

}

int main(){

    //test_doble_free();
    //test_acceso_despues_de_free();
    //test_acceso_dir_local();

    //test_overflow_en_heap();
    //test_underflow_en_heap();
    //test_overflow_en_stack();

    //test_laguna_memoria();
    //test_fallo_realloc();

    //test_overflow_string();
    //test_free_var_en_stack();

    test_acceso_no_init();

    return 0;
}
