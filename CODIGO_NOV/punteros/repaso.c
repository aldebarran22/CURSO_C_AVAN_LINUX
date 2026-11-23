// Repaso de punteros
// Operadores, aritmetica, trabajo con arrays

// git  clone https://github.com/aldebarran22/CURSO_C_AVAN_LINUX /curso
// git pull


#include  <stdio.h>


void operadores(){
    int num = 100;
    int *p = &num;

    printf("\n dir: %p - %d", p, *p);
    printf("\n el puntero: %u bytes, int %u bytes ", sizeof(p), sizeof(*p));
}

int main(){
    operadores();
    return 0;
}