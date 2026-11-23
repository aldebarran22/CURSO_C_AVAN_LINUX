// Punteros a funcion

#include <stdio.h>

// Declarar el prototipo:
int sum(int, int);
int dif(int, int);
int mul(int, int);

typedef int (*operacion_t)(int, int);


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