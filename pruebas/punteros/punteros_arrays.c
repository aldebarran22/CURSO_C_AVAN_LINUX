// Pruebas con los punteros y arrays

#include <stdio.h>
#include <string.h>


void prueba1(){
    int numeros[] =  {1,2,3,4,5};
    char mensaje[] = {"hola que tal"};
    int* ptr;
    int* ptr2;

    // El nombre del array es un puntero al primer elemento:
    // Para recogerlo:
    ptr = numeros;
    ptr2 = &numeros[0];

    printf("\n ptr = %p, *ptr = %d", ptr, *ptr);
    printf("\n ptr2 = %p, *ptr2 = %d", ptr2, *ptr2);

    // Formas de acceder al contenido del array:
    int n = sizeof(numeros) / sizeof(int);

    puts("\n\nContenido del Array de int:");
    // El salto es proporcional al tamaño del tipo:
    for (int i = 0 ; i < n ; i++){
        printf("%d %d %p %p\n", numeros[i], ptr[i], &numeros[i], numeros+i);
    }

    puts("\n\nContenido del Array de char:");
    int n2 = strlen(mensaje);
    char* s = mensaje;

    // El salto es proporcional al tamaño del tipo:
    for (int i = 0 ; i < n2 ; i++){
        printf("%c %c %p %p\n", mensaje[i], s[i], &s[i], s+i);
    }

}


int main(){
    prueba1();

    puts("");
    return 0;
}