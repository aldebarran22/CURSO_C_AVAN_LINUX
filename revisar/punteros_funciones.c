// Punteros a función

#include <stdio.h>

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
  	// Devuelve el pto. a función a partir de un parámetro.
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
	// inserción directa
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
	// Definición de un puntero a la función sumar:
	int (*psumar)(int, int);
	
	// se inicializan al nombre de la función:
	psumar = sumar;
	
	// Llamada a la función a través del puntero:
	printf("sumar: %d", psumar(55,77));
	
	// Definir un puntero para la función operacion:
	char *(*ptr)(const char *, int);
	
	ptr = operacion;
	ptr("hola",4);
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
}

void testOrdenacion(){
	double arr[] = {5.7, 1.999, 0.7, -4.6, 2.33};
	
	ordenacionBurbuja(arr, 5);
	imprimir(arr, 5);
	
	ordenacionBurbujaPF(arr, 5, asc);
	imprimir(arr, 5);
	
	ordenacionBurbujaPF(arr, 5, desc);
	imprimir(arr, 5);
}

void ordenarArray(double *arr, int size, void(*metodo)(double *, int) ){
	metodo(arr, size);
}

void testMetodosOrdenacion(){
	// Implementar una funcion que reciba el array, size y el método de ordenación que tiene que aplicar
	double arr[] = {5.7, 1.999, 0.7, -4.6, 2.33};
	
	ordenarArray(arr, 5, ordenacionBurbuja);
	imprimir(arr, 5);
	
	ordenarArray(arr, 5, ordenacionInsertDirecta);
	imprimir(arr, 5);
	
}


int main(){
	
	testSeleccionarOperacion();
	return 0;
}








