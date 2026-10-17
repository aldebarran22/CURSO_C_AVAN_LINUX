#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t type;
    uint32_t length;
} header_t;

#pragma pack(pop)

int main(void)
{
    printf("Tamaño: %zu\n", sizeof(header_t));
    printf("Offset type: %zu\n", offsetof(header_t, type));
    printf("Offset length: %zu\n", offsetof(header_t, length));

    return 0;
}
