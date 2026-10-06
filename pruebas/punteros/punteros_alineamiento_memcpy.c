
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


static void alineamientos(void){
    char buffer[9] = "12345678";
    int32_t value;

    memcpy(&value, buffer + 1, sizeof(value));

    // PRId32 macro formato para printf
    // Es para imprimir un valor correcto de int32_t
    printf("Valor decimal: %" PRId32 "\n", value);
    printf("Valor hexadecimal: 0x%08" PRIx32 "\n",(uint32_t)value);

    printf("Cadena: %s\n", buffer + 1);
    printf("Primer carácter: %c\n", buffer[1]);

    //PRId32: decimal con signo.
    //PRIu32: decimal sin signo.
    //PRIx32: hexadecimal en minúsculas.
    //PRIX32: hexadecimal en mayúsculas.
}


int main(void){
    alineamientos();
    return 0;
}