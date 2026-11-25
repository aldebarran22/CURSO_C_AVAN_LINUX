#include <stdio.h>


int main(){
    unsigned char b;

    for (b = 0 ; b <= 255 ; b++){
        printf("\n%03d : %c", (int)b, b);
    }
}