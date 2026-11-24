// Implementación de las funciones:

#include <stdio.h>
#include "funciones.h"

// Inicializar la variable externa
int numFunciones = 3;

static int sum(int a, int b){
    return a+b;
}

int dif(int a, int b){
    return a-b;
}

int mul(int a, int b){
    return a*b;
}

int (*seleccionarOp2(int op))(int, int){
    switch (op)
    {
    case SUM:
        return sum;

    case DIF:
        return dif;

    case MUL:
        return mul;    
    
    default:
        return NULL;
    }
}

operacion_t seleccionarOp(int op){
    switch (op)
    {
    case SUM:
        return sum;

    case DIF:
        return dif;

    case MUL:
        return mul;    
    
    default:
        return NULL;
    }
}


