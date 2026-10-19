
// Repaso de punteros

#include <stdio.h>

void operadores(){
    int num=100;

    int* ptr = &num;

    printf("\n dir: %p - %d", ptr, *ptr);
    printf("\n %u bytes y el int %u: ", sizeof(int*), sizeof(int));
}

void imprimir_mal(int *ptr){
    // Esta mal porque el puntero ocupa 8bytes al div por sizeof de int solo accede a los dos primeros
    // elementos del array:
    int n = sizeof(ptr) / sizeof(int);

    printf("\n\nIMPRIMIR: ptr ocupa %d bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %d - %p", i, *(ptr+i), ptr+i);
    }
}


void imprimir(int *ptr, int n){

    printf("\n\nIMPRIMIR: ptr ocupa %d bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %d - %p", i, *(ptr+i), ptr+i);
    }
}

void modifica(int *p){
    printf("\nmodifica dir %p", p);
    *p += 100;
}

void modifica_arr(int *ptr, int n){
    for (int i = 0 ; i < n ; i++){
        modifica(&ptr[i]);
    }
}

void arrays(){
    int num[] = {1,2,3,4,5};

    int *ptr = num;
    int *ptr2 = &num[0];

    printf("\n %d - %d", *ptr, *num);
    printf("\n %d - %d", *(ptr+2), num[2]);

    int n = sizeof(num) / sizeof(int);

    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %d - %p - %d - %p", i, *(ptr+i), ptr+i, *(num+i), num+i);
    }

    imprimir(num, n);
}

int *meses_mal(){
    // Error, devuelve la dir de una variable local, que se destruye cuando termina
    // la ejecucion de la funcion.

    // OJO, el array se crea  en la stack no el heap.
    int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    return dias;
}


int main(){
    //operadores();
    //arrays();

    /*
    int num = 10;
    modifica(&num);
    printf("\nnum = %d", num);

    int numeros[] = {1,2,3,4,5};
    modifica_arr(numeros, 5); // tipo de num: int *
    //modifica_arr(&numeros, 5);// tipo de num: int **
    imprimir(numeros, 5);
    */

    //imprimir(meses(), 12); // error de ejecucion.

    puts("");
    return 0;
}