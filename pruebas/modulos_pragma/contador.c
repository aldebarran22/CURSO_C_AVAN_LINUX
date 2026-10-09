

#include <stdio.h>

#include "contador.h"

// Aqui es donde se define la variable  externa

int contador_global = 0;

// La variable global static, solo es accesible
// desde este fichero
static int numero_operaciones = 0;

// Funcion privada del modulo:
// Si se define como static no se puede invocar desde
// otro modulo

static void registrar_operacion(void){
    numero_operaciones++;
}


// Funcion publica:
// Esta declarada en contador.h y puede invocarse
// desde cualquier fichero que incluya la  cabecera
void contador_incrementar(void){
    contador_global++;
    registrar_operacion();
}

void contador_mostrar(){
    printf("\nContador global: %d\n", contador_global);
    printf("\nNumero de operaciones: %d\n", numero_operaciones);

}

