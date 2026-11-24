#include <stdio.h>

#include "tipos.h"
#include "funciones.h"


void testFunciones(){

    operacion_t pFun = seleccionarOp(SUM);
    printf("\nSuma: %d", pFun(12,45));
    //printf("\nSuma: %d", sum(12,45));

    printf("\nNum funciones disponibles: %d", numFunciones);
}

void  testOrdenaciones(){

}


int main(){
    testFunciones();
    testOrdenaciones();
    puts("");
    return 0;
}