

#include <stdio.h>
#include <stdint.h>



typedef struct {
    uint8_t type;
    uint32_t length;
} header_t;



int main(){
    // Size de la estructura:
    printf("\nheader_t ocupa %zu bytes", sizeof(header_t));

    puts("");
    return 0;
}