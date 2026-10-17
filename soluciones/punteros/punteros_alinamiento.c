// Alineamiento de punteros

#include <stdio.h>
#include <stdint.h>


void alineamientos(){
    // El alineamiento se garantiza a 1 byte:
    char buffer[8+1]= {"12345678"};

    // Puede  requerir un alineamiento de 4 bytes
    int32_t* p;

    // Los 4 bytes leidos caen dentro  de chars    
    p = (int32_t*)(buffer+1);


    printf("\n%d %s %c", *p, (char*)p, *p);
    printf("\nHex: %X", *p);

    // Se intentan leer 4 bytes del entero.
    // Litle endian: menor peso en menor direccion
    // Big-endian: mayor peso en menor direccion.
    


}


int main(){
    alineamientos();

    puts("");
    return 0;
}