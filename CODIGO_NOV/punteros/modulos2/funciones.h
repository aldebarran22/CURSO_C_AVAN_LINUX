

#ifndef FUNCIONES_H

    #define FUNCIONES_H

    #include "tipos.h"

    #define SUM 0
    #define DIF 1
    #define MUL 2
    #define DIV 3

    // Variable externa:
    extern int numFunciones;

    // Declarar el prototipo:
    static int sum(int, int);
    int dif(int, int);
    int mul(int, int);

    int (*seleccionarOp2(int))(int, int);
    operacion_t seleccionarOp(int op);

#endif