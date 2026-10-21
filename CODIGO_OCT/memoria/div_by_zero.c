// Ejemplo, de un sanitizer para la div.
// por cero

/*
gcc -std=c11 \
-Wall -Wextra -Wpedantic \
-g -O1 \
-fsanitize=undefined \
-fno-omit-frame-pointer \
div_by_zero.c \
-o div_by_zero


*/

#include <stdio.h>

void div0(int a, int b){
    printf("\nValor %d", a / b);
}

int main(){
    div0(8, 0);
    return 0;
}