
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

void imprimir(int *ptr, int n){

    printf("\n\nIMPRIMIR: ptr ocupa %d bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %d - %p", i, *(ptr+i), ptr+i);
    }
}

int *meses(){
    // OJO no se destruye el array:
    static int dias[] = {1,2,3,4, 5};

    return dias;
}


int main(){
    for (int i = 0 ; i < 3 ;  i++){
        inc();
    }

    for (int i = 0 ; i < 3 ;  i++){
        inc2();
    }

    int *dias = meses();
    imprimir(dias, 5);

    puts("");
    return 0;
}