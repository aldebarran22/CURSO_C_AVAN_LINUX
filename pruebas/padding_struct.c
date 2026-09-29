#include <stdin.h>
#include <stddef.h>


typedef struct {
	uint8_t type;
	uint32_t identifier;
} mensaje_t;

int main(){
	printf("Tamaño: %zu\n", sizeof(message_t));
	printf("Offset type: %zu\n", offsetof(message_t, type));
	printf("Offset identifier: %zu\n",
	offsetof(message_t, identifier));
}
