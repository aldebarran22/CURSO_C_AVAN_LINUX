// Punteros a funcion

#include <stdio.h>

#define SUM 0
#define DIF 1
#define MUL 2
#define DIV 3


// Declarar el prototipo:
int sum(int, int);
int dif(int, int);
int mul(int, int);

typedef int (*operacion_t)(int, int);


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

int operar2(operacion_t operacion, int param1, int param2){
    return operacion(param1, param2);
}


int operar(int (*operacion)(int, int), int param1, int param2){
    return operacion(param1, param2);
}




void definirPtoFunc(){
    int (*pFuncion)(int, int);
    
    pFuncion = sum;
    printf("\nSuma: %d", pFuncion(1,2));

    pFuncion = mul;
    printf("\nMultiplicar: %d", pFuncion(1,2));

    printf("\nResultado con operar: %d", operar(sum, 1,2));
    printf("\nResultado con operar2: %d", operar2(sum, 1,2));

    operacion_t ptr = seleccionarOp(DIV);
    if (ptr != NULL){
        printf("\nResultado mul: %d", ptr(23,45));

    } else {
        puts("\nOperacion no valida");
    }
}


int main(){
    printf("\nDir de suma: %p", sum);
    definirPtoFunc();
    puts("");
    return 0;
}

int sum(int a, int b){
    return a+b;
}

int dif(int a, int b){
    return a-b;
}

int mul(int a, int b){
    return a*b;
}