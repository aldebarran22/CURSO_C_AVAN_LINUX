// Repaso de punteros
// Operadores, aritmetica, trabajo con arrays

// git  clone https://github.com/aldebarran22/CURSO_C_AVAN_LINUX /curso
// git pull


#include  <stdio.h>


void operadores(){
    int num = 100;
    int *p = &num;

    printf("\n dir: %p - %d", p, *p);
    printf("\n el puntero: %lu bytes, int %lu bytes ", sizeof(p), sizeof(*p));
}


void imprimir(int *ptr){
    // ojo, el puntero ocupa  8 bytes, solo vemos 2 numeros!
    int n = sizeof(ptr) / sizeof(int);
    for (int i = 0 ; i < n ; i++){
        printf("\n %d = %d", i, ptr[i]);
    }
}

void imprimir_ok(int *ptr, int n){
    // ok    
    for (int i = 0 ; i < n ; i++){
        printf("\n %d = %d %d %p", i, ptr[i], *(ptr + i), (ptr+i));
    }
}

int main(){
    //operadores();
    int numeros[] = {1,2,3,4,5};
    int n = sizeof(numeros) / sizeof(int);
    imprimir(numeros);
    puts("");
    imprimir_ok(numeros, n);
    puts("");
    return 0;
}