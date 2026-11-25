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
./errores_memoria2


gcc -std=c11 -g \
-fsanitize=address,undefined \
-fno-omit-frame-pointer \
detectar_errores.c -o detectar_errores

*/

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
}


int main(){

    test_doble_free();
    return 0;
}
