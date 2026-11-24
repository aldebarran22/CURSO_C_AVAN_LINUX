
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint8_t byte; // Añade 3  bytes de padding
    uint32_t reg;
} header_t; // 4 + 4 = 8 bytes


#pragma pack(push,1)
typedef struct {
    uint8_t byte; 
    uint32_t reg;
} header2_t; // 1 + 4 = 5 bytes
#pragma pack(pop)


int main(){
    printf("\nheader_t ocupa: %lu bytes", sizeof(header_t));
    printf("\nheader2_t ocupa: %lu bytes", sizeof(header2_t));
    printf("\nOffset type: %zu", offsetof(header_t, byte));
    printf("\nOffset length: %zu", offsetof(header_t, reg));
    puts("");
    return 0;
}