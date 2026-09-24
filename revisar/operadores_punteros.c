// Operadores de los punteros, paso de parámetros ...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void intercambiarVars(float *a, float *b){
	float aux = *a;
	*a = *b;
	*b = aux;
}


void imprimir(int *array, int n){
	int i;
		
	for (i=0; i <  n; i++)
		printf("%d  ", array[i]);
	puts("");
}

void intercambiarPunteros(int **a, int **b){
	printf("Dir a: %p, dir b: %p\n\n",a, b);
	printf("Primer numero de a: %d\n", **a);
	printf("Primer numero de b: %d\n", **b);
	
	// (*a) accede al primer array, se suma el desplazamiento (+3)  y se accede al contenido con el * exterior	
	printf("cuarto numero de a: %d\n", *((*a)+3)) ;
	printf("cuarto numero de b: %d\n", *((*b)+3)) ;
	int *aux = *a;
	*a = *b;
	*b = aux;
}

void testOperadores(){
	// Operadores & y *
	double real = 45.778;
	
	// Extraer la dir.
	double *ptr = &real;
	// %g quita el sobrante de ceros!
	printf("%g %g\n", real, *ptr); // *ptr accede al contenido de la variable.
	
	float a = 5.4F, b = 6.7F;
	printf("Dir a: %p, dir b: %p\n", &a, &b);
	intercambiarVars(&a, &b);
	printf("%f %f\n", a,b);
	
	int numA[] = {1,2,2,4};
	int numB[] = {5,6,7,8};
	
	
	int *ptrA, *ptrB;
	ptrA = numA;
	ptrB = numB;
	printf("Primer numero de a en main: %d\n", *ptrA);
	printf("Primer numero de b en main: %d\n", *ptrB);
	printf("Primer numero de a en main: %d\n", *numA);
	printf("Primer numero de b en main: %d\n", *numB);

	
	printf("Dir A: %p\n", ptrA);
	printf("Dir B: %p\n", ptrB);	
	
	imprimir(ptrA, 4);
	imprimir(ptrB, 4);
	puts("");
	
	intercambiarPunteros(&ptrA, &ptrB);
	
	imprimir(ptrA, 4);
	imprimir(ptrB, 4);
}

void testPasoParametros(){
	
}



int main(){
	testOperadores();
	//testPasoParametros();
}
