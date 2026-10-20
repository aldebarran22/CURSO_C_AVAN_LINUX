

// stdint: tipo uint8_t, uint16_t ...
// stddef: offset dentro de los campos de una struct

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>



typedef struct {
    uint8_t type;
    uint32_t length;
} header_t;



int main(){
    // Size de la estructura:
    printf("\nheader_t ocupa %zu bytes", sizeof(header_t));
    printf("\nOffset type: %zu", offsetof(header_t, type));
    printf("\nOffset length: %zu", offsetof(header_t, length));

    puts("");
    return 0;
}