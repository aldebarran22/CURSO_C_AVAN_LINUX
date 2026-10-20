

#include <stdio.h>

#include "configuracion.h"
#include "contador.h"

// Definicion real de la variable declarada
// como extern dentro del modulo:

int incremento_configurado = 1;

// Variable privada de configuracion;
static int cambios_configuracion = 0;


// Funcion privada solo para este modulo:
static int incremento_es_valido(int incremento){
    return incremento > 0 && incremento <= 100;
}

void configuracion_establecer_incremento(int incremento){
    if (!incremento_es_valido(incremento)){
        printf("\nEl incremento no es valido: %d\n", incremento);
        return;
    }

    incremento_configurado = incremento;
    cambios_configuracion++;
}

void configuracion_aplicar_incremento(void){
    contador_global = incremento_configurado;

}

void configuracion_mostrar(){
    // Contador global definido en contador.c
    // y declarado como extern en contador.h
    printf("\nIncremento configuracion: %d\n", incremento_configurado);
    printf("\nCambios configuracion: %d", cambios_configuracion);
}

