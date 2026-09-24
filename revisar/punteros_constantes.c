// Punteros ctes. y punteros a ctes.

#include <stdio.h>


void testPunterosCtes(){
	// Se puede cambiar el contenido pero no el puntero:
	int x = 10, y = 20;
	int * const px = &x;
	
	// Incrementar el 1 el contenido de px: no hay problema!
	(*px)++;
	
	printf("\nx = %d, px = %p\n\n", x, px);
	
	// Cambiar el puntero, cambiar la dirección a la que apunta:
	//px = &y; No podemos cambiar la dirección
}


void testPunteros_A_Ctes(){
	const char *path = "empleados.csv";
	
	puts(path);
	// Intentamos modificar el contenido:
	//path[0] = 'E';  OJO no podemos cambiar el contenido del puntero.

	int x = 10, y = 20;
	const int * px = &x;
	
	printf("\nx = %d, px = %p", x, px);
	px = &y;
	printf("\nx = %d, px = %p\n\n", x, px);
	//(*px)++; No podemos cambiar el contenido del puntero.
}


int main(){
	testPunterosCtes();
	testPunteros_A_Ctes();
	return 0;
}
