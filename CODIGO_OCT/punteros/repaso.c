
// Repaso de punteros

#include <stdio.h>

void operadores(){
    int num=100;

    int* ptr = &num;

    printf("\n dir: %p - %d", ptr, *ptr);
    printf("\n %u bytes y el int %u: ", sizeof(int*), sizeof(int));
}

void arrays(){
    int num[] = {1,2,3,4,5};

    int *ptr = num;
    int *ptr2 = &num[0];

    printf("\n %d - %d", *ptr, *num);
}


int main(){
    operadores();

    return 0;
}