

// stdint: tipo uint8_t, uint16_t ...
// stddef: offset dentro de los campos de una struct

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


// Rellena un padding de 3 bytes al uint8_t: ocupa 4+4 = 8 bytes
typedef struct {
    uint8_t type;
    uint32_t length;
} header_t;

// Empaqueta a un 1 bytes. Ocupa: 1 + 4 = 5 bytes
#pragma pack(push, 1)
typedef struct {
    uint8_t type;
    uint32_t length;
} header_t2;
#pragma pack(pop)


int main(){
    // Size de la estructura:
    printf("\nheader_t ocupa %zu bytes", sizeof(header_t));
    printf("\nOffset type: %zu", offsetof(header_t, type));
    printf("\nOffset length: %zu", offsetof(header_t, length));
    
    puts("");

    printf("\nheader_t2 ocupa %zu bytes", sizeof(header_t2));
    printf("\nOffset type: %zu", offsetof(header_t2, type));
    printf("\nOffset length: %zu", offsetof(header_t2, length));

    puts("");
    return 0;
}