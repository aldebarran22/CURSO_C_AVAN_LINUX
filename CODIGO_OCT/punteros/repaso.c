
// Repaso de punteros

#include <stdio.h>

void operadores(){
    int num=100;

    int* ptr = &num;

    printf("\n dir: %p - %d", ptr, *ptr);
    printf("\n %u bytes y el int %u: ", sizeof(int*), sizeof(int));
}

void imprimir(int *ptr){
    int n = sizeof(ptr) / sizeof(int);

    printf("\n\nIMPRIMIR: ptr ocupa %d bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %d - %p", i, *(ptr+i), ptr+i);
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

    imprimir(num);
}


int main(){
    //operadores();
    arrays();

    puts("");
    return 0;
}