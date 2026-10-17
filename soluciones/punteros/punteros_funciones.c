// Punteros a funcion

#include <stdio.h>
#include <stdlib.h>


#define SUMA 0
#define RESTA 1
#define MUL 2


int sumar(int a, int b){
	return a+b;
}

int restar(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}


int (*seleccionarOperacion(int op))(int,int){
  	// Devuelve el pto. a funcion a partir de un parametro.
  	switch(op){
  		case SUMA:
  			return sumar;
  			
  		case RESTA:
  			return restar;
  			
  		case MUL:
  			return mul;
  			
  		default:
  			return NULL;
	}
}

char *operacion(const char *s, int p){
	printf("\noperacion: %s, p = %d", s,p);
	return NULL;
}

void ordenacionBurbuja(double *array, int size){
	// burbuja
	int i;
	int j;
	double aux_elem;
	
	for (i = 0; i < size - 1; i++){
	
		for (j = i+1; j < size; j++){
		
			if (array[i] > array[j]){
			
				aux_elem = array[i];
				array[i] = array[j];
				array[j] = aux_elem;
			}
		}
	}
}

void ordenacionInsertDirecta(double *array, int size){
	// insercion directa
	int i, j, min;
	double str;

	for (i = 0; i < size-1; i++)
	{
		min = i;
		for (j=i+1 ; j < size ; j++){
			if (array[j] < array[min])
				min = j;
		}
	   
		str = array[i];
		array[i] = array[min];
		array[min] = str;
	}
}

int asc(int a, int b){
	return a > b;
}

int desc(int a, int b){
	return a < b;
}

void ordenacionBurbujaPF(double *array, int size, int (*criterio)(int,int) ){
	// burbuja
	int i;
	int j;
	double aux_elem;
	
	for (i = 0; i < size - 1; i++){
	
		for (j = i+1; j < size; j++){
		
			if (criterio(array[i], array[j])) {			
				aux_elem = array[i];
				array[i] = array[j];
				array[j] = aux_elem;
			}
		}
	}
}

void testPunterosFuncion(){
	// Definicion de un puntero a la funcion sumar:
	int (*psumar)(int, int);
	
	// se inicializan al nombre de la funci�n:
	psumar = sumar;
	
	// Llamada a la funci�n a trav�s del puntero:
	printf("sumar: %d", psumar(55,77));
	
	// Definir un puntero para la funci�n operacion:
	char *(*ptr)(const char *, int);
	
	ptr = operacion;
	ptr("hola",4);
	puts("");
}


void imprimir(double *array, int n){
	int i;
		
	for (i=0; i <  n; i++)
		printf("%g ", array[i]);
	puts("");
	
}


void testSeleccionarOperacion(){
	int (*ptr)(int,int);
	int a = 20, b = 2;
	
	ptr = seleccionarOperacion(SUMA);
	printf("\nSuma: %d", ptr(a,b));
	
	ptr = seleccionarOperacion(RESTA);
	printf("\nResta: %d", ptr(a,b));
	
	ptr = seleccionarOperacion(MUL);
	printf("\nMul: %d", ptr(a,b));

	puts("");
}


void testOrdenacion(){
	double arr[] = {5.7, 1.999, 0.7, -4.6, 2.33};
	
	ordenacionBurbuja(arr, 5);
	imprimir(arr, 5);
	
	ordenacionBurbujaPF(arr, 5, asc);
	imprimir(arr, 5);
	
	ordenacionBurbujaPF(arr, 5, desc);
	imprimir(arr, 5);

	puts("");
}


void ordenarArray(double *arr, int size, void(*metodo)(double *, int) ){
	metodo(arr, size);
}


void testMetodosOrdenacion(){
	// Implementar una funcion que reciba el array, size y el m�todo de ordenaci�n que tiene que aplicar
	double arr[] = {5.7, 1.999, 0.7, -4.6, 2.33};
	
	ordenarArray(arr, 5, ordenacionBurbuja);
	imprimir(arr, 5);
	
	ordenarArray(arr, 5, ordenacionInsertDirecta);
	imprimir(arr, 5);
	
	puts("");
}


int compare(const void *p1, const void *p2){
	// Retornar 0, -1, o +1
	double d1 = *(double *)p1;
	double d2 = *(double *)p2;

	if (d1 == d2){
		return 0;

	} else if (d1 < d2){
		return -1;

	} else {
		return 1;
	}

}


void test_qsort(){
	double arr[] = {5.7, 1.999, 0.7, -4.6, 2.33};
	int n = sizeof(arr) / sizeof(double);

	imprimir(arr, n);
	qsort(arr, n, sizeof(double), compare);
	imprimir(arr, n);
	puts("");
}


int main(){
	test_qsort();	
	testSeleccionarOperacion();
	testPunterosFuncion();
	testOrdenacion();
	testMetodosOrdenacion();
	test_qsort();
	return 0;
}

