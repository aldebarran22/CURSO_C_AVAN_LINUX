// Punteros a funcion

#include <stdio.h>

// Declarar el prototipo:
int sum(int, int);
int dif(int, int);
int mul(int, int);


void definirPtoFunc(){
    int (*pFuncion)(int, int);
    
    pFuncion = sum;
    printf("\nSuma: %d", pFuncion(1,2));

    pFuncion = mul;
    printf("\nMultiplicar: %d", pFuncion(1,2));
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