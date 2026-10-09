
// Programa principal:
#include <stdio.h>

#include "contador.h"
#include "configuracion.h"

int main(){
    puts("Estado inicial");


    contador_mostrar();
    configuracion_mostrar();

    puts("\nIncrementando mediante el modulo contador:");
    contador_incrementar();
    contador_incrementar();

    contador_mostrar();
    puts("\nConfigurando un incremento de 5");
    configuracion_establecer_incremento(5);
    configuracion_aplicar_incremento();

    contador_mostrar();
    configuracion_mostrar();

    // Acceso directo a las variables globales
    printf("\nAcceso desde main: contador_global = %d\n", contador_global);
    printf("\nAcceso desde main: incremento_configurado = %d\n", incremento_configurado);

    /*
     * Estas operaciones no seran posibles:
     *
     * numero_operaciones = 20;
     * registrar_operacion();
     * cambios_configuracion = 10;
     * incremento_es_valido(5);
     *
     * Todos estos elementos son static y privados
     * de sus respectivos ficheros fuente.
     */

     // gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 main.c contador.c configuracion.c -o programa

     // Estandar: c11
     
     // -Wall activar advertencias, variables sin utilizar, funciones declaradas y no usadas
     // formatos incorrectos en printf
     
     // -Wextra activar advertencias adicionales que no estan en -Wall
     // por ejemplo: parametros de funciones que no se utilizan
     
     //-Wedantic: advertencias cuando el codigo utiliza caracteristicas que no se cumplen

     // -g Añade al ejecutable informacion de depuracion
     // para que muestre: nombre de funciones y variables, numero  de linea y pila de llamadas

     //-0O Desactivar optimizaciones


    return 0;
}