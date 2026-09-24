// Ejemplos de macros en C

#include <stdio.h>

#define MENOR(A,B) ((A<B)?A:B)
#define N 100

#ifdef MENOR
	#warning ojo hay una macro con parametros
#endif

#ifndef SUMA
	#error no se ha definido suma
#endif


void testMenor(){
	int a = 9, b = 4;
	char c1 = 'A', c2 = 'a';
	
	printf("Menor int: %d\n", MENOR(a,b));
	printf("Menor char: %c\n", MENOR(c1,c2));
	printf("Menor float: %f\n", MENOR(2.5F,7.9F));
	
	printf("\na = %d, b = %d", a,b);
	MENOR(a--, b--);
	printf("\na = %d, b = %d", a,b);
	
	printf("\nN = %d", N);
	
	//#undef N
	
	//printf("\nN = %d", N);
	
}


int main(){
	testMenor();
	return 0;
}
