// Punteros void, ejemplos

#include <stdio.h>

void prueba1(){
    int x = 10;

    // Cualquier puntero se puede castear a void*
    void* p =  &x;

    // *p = 20; // ERROR void no tiene size

    printf("\nx = %d", x);
    
    // Para poder modificar hay que hacer casting a int*
    *((int *)p) = 20;

    printf("\nx = %d", x);
}

void prueba2(void *p){
    // Antes de acceder a  memoria hay que hacer el casting
    int* n = (int*) p;

    printf("\nNumero: %d", *n);

}

void recorrerBytes(const void *buf, size_t n){
    // Recorre en little endian
    unsigned char *p = buf;
    puts("\nRecorrer bytes:");
    for (int i = n-1 ; i >= 0 ; i--){
        printf("%02X", p[i]);
    }
    puts("");
}


int main(){
    int num = 123;

    //prueba1();
    //prueba2(&num);
    recorrerBytes(&num,  sizeof(int));
    printf("\nnum: %X", num);
    puts("");
    return 0;
}