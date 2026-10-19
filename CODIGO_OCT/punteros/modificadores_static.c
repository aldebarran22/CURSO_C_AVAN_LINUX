
#include <stdio.h>


void inc(){
    int i = 0;

    i++;
    printf("\ni = %d", i);
}

void inc2(){
    // ojo no se destruye!
    static int i = 0;

    i++;
    printf("\ni = %d", i);
}


int main(){
    for (int i = 0 ; i < 3 ;  i++){
        inc();
    }

    for (int i = 0 ; i < 3 ;  i++){
        inc2();
    }

    puts("");
    return 0;
}