#include <stdio.h>


int main(){
    /*
    
    gcc -std=c11 \
    -Wall -Wextra -Wpedantic \
    -g -O1 \
    -fsanitize=undefined \
    -fno-omit-frame-pointer \
    bucle_infinito.c \
    -o bucle_infinito
        
    */


    unsigned char b; // 1 byte:  0000 0000 al ffff ffff
    // 256: 1 0000 0000

    for (b = 0 ; b <= 255 ; b++){
        printf("\n%03d : %c", (int)b, b);
    }
}