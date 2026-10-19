

// Punteros a funcion, pasar funciones como parametros, funciones que devuelven 
// funciones (punteros a funcion)

#include <stdio.h>
#include <stdlib.h>


int suma(int a, int b){
    // Prototipo: int suma(int, int);
    return a+b;
}


double funcion(const char *s){
    return 0.0;
}


void definirPtrFuncion(){
    printf("\nFuncion suma: %p", suma);

    // Definir un ptr a funcion a suma:
    int (*psuma)(int, int);

    // Asignarlo:
    psuma = suma;

    // Ejecutar a través del ptr:
    printf("\n psuma: %d", psuma(2,5));
}


void imprimir(double *ptr, int n){

    printf("\n\nIMPRIMIR: ptr ocupa %zu bytes", sizeof(ptr));
    for (int i = 0 ; i < n ; i++){
        printf("\n %d - %g - %p", i, *(ptr+i), ptr+i);
    }
}

int criterio(const void *p1, const void *p2){
    // 0 iguales, -1 *p1 < *p2, +1 *p1 > *p2
    double d1 = *(double *)p1;
    double d2 = *(double *)p2;

    if (d1 == d2){
        return 0;

    } else if (d1 < d2){
        return 1;

    } else if (d1 > d2){
        return -1;
    }
}

void ordenarArray_qsort(){
    double numeros[] = {5.7, 8.88, -9.9, 0.0, 123.6};

    int n = sizeof(numeros) / sizeof(double);
    imprimir(numeros, n);

    // el array, cuantos elems, bytes de cada elem, funcion criterio.
    qsort(numeros, n, sizeof(double), criterio);
    imprimir(numeros, n);
}

void asignarPtrFunc_mal(){
    // OJO asignamos una funcion al puntero que no cumple el prototipo.
    int (*psuma)(int, int);

    psuma =  funcion;
    printf("\npsuma = %d", psuma(3,4));
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


int main(){
    //definirPtrFuncion();
    //ordenarArray_qsort();

    asignarPtrFunc_mal();

    puts("");
    return 0;
}