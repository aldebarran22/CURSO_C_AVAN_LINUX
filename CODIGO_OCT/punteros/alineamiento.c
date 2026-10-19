// Ejemplo con el alineamiento:


#include <stdio.h>
#include <inttypes.h>


void acceso_desalineado(){
    char buf[8+1] = {"01234567"}; // 0 .. 7
    // Estamos saltando el primer byte!
    // bytes 1,2,3,4
    int32_t *p = (int32_t *)(buf + 1);
    printf("\n int32: %d\n", *p);

}

void alineamientos(void){
    /*
     * Array de 9 caracteres:
     *
     * Índice:    0   1   2   3   4   5   6   7   8
     * Contenido: 1   2   3   4   5   6   7   8  \0
     *
     * Los elementos char requieren alineamiento de 1 byte.
     * Todos los caracteres están almacenados consecutivamente,
     * sin bytes de relleno entre ellos.
     */
    char buffer[8 + 1] = "12345678"; // '\0'

    /*
     * p es un puntero a int32_t.
     *
     * int32_t ocupa exactamente 4 bytes cuando está disponible,
     * pero su alineamiento depende de la plataforma.
     * Es frecuente que requiera una dirección múltiplo de 4.
     */
    int32_t *p;

    /*
     * buffer + 1 apunta al carácter '2'.
     *
     * Si buffer comenzase, por ejemplo, en 0x1000:
     *
     * buffer     -> 0x1000
     * buffer + 1 -> 0x1001
     *
     * 0x1001 no es múltiplo de 4, por lo que posiblemente
     * no está correctamente alineado para int32_t.
     *
     * Además, el objeto real es un array de char, no un int32_t.
     * Acceder a esos bytes mediante int32_t también viola las
     * reglas de tipos efectivos o strict aliasing.
     */
    p = (int32_t *)(buffer + 1);

    /*
     * *p intenta leer cuatro bytes desde buffer*
     * '2'  '3'  '4'  '5'
     *
     * Pero esta desreferenciación tiene comportamiento indefinido.
     * Puede funcionar aparentemente, producir otro resultado
     * o provocar un error de alineamiento en ciertas arquitecturas.
     */
    printf("Valor decimal: %" PRId32 "\n", *p);

    /*
     * Convertir p a char * sí permite leer los bytes como caracteres.
     * Como apunta a buffer[1], se imprimiría normalmente:
     *
     * 2345678
     *
     * porque el array contiene un terminador nulo al final.
     */
    printf("Como cadena: %s\n", (char *)p);

    /*
     * %c imprime solamente el byte menos significativo del valor
     * entero obtenido mediante *p.
     *
     * No significa necesariamente que vaya a imprimir '2'.
     * El resultado depende del orden de bytes y, previamente,
     * ya existe comportamiento indefinido.
     */
    printf("Como carácter: %c\n", (int)*p);

    /*
     * Muestra el valor leído en hexadecimal.
     * De nuevo, la lectura mediante *p no es válida.
     * 
     * PRI  Formato para printf
        d    Entero decimal con signo
        32   Tipo entero de 32 bits
     */
    printf("Hexadecimal: %08" PRIX32 "\n",(uint32_t)*p);
}





int main(){
    //acceso_desalineado();
    alineamientos();

    puts("");
    return 0;
}