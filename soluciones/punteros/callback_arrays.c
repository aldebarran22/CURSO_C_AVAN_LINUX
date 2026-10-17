// Definir un array de callback

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int(*callback_t)(int, int);

int sum(int a, int b){
    return a+b;
}


int dif(int  a, int b){
    return a-b;
}


int mul(int a, int b){
    return a*b;
}


int operaciones(callback_t op, int a, int b){
    return op(a, b);
}


callback_t buscarOperacion(const  char* operacion){
    callback_t operaciones[] = {sum, dif, mul};
    const char* nombres[] = {"sum", "dif", "mul"};

    for (int i = 0 ; i < 3 ; i++){
        if (!strcmp(nombres[i], operacion)){
            return operaciones[i];
        }

    }
    return NULL;
}


int main(){
    const char* opera = "mul";
    int a = 20, b = 9;

    printf("%s con %d y %d = %d", opera, a, b, operaciones(buscarOperacion(opera), a, b));
    puts("");
    return 0;
}