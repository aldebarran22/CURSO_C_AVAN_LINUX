#include <stdio.h>

#define SUMA 0

void funcion(){
    int y;
    int z = 100;
}


int main(){
    int x;

    if (SUMA == 0){
        puts("Existe suma");
    }

    #undef SUMA

    // A partir de aqui la macro SUMA no existe!
    /*
    if (SUMA == 0){
        puts("Existe suma");
    }*/

    return 1;
}