

// Punteros a funcion, pasar funciones como parametros, funciones que devuelven 
// funciones (punteros a funcion)

#include <stdio.h>
#include <stdlib.h>

#define SUMA 0
#define RESTA 1
#define MUL 2


int suma(int a, int b){
    // Prototipo: int suma(int, int);
    return a+b;
}

int restar(int a, int b){
    // Prototipo: int restar(int, int);
    return a-b;
}

int mul(int a, int b){
    // Prototipo: int mul(int, int);
    return a*b;
}


int (*seleccionarOperacion(int op))(int, int){
    // Devuelve un puntero a funcion a partir del parametro:
    switch(op){
        case SUMA:
            return suma;

        case RESTA:
            return restar;

        case MUL:
            return mul;

        default:
            return NULL;

    }
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

    printf("\n\nIMPRIMIR:\n");
    for (int i = 0 ; i < n ; i++){
        printf("%g\t", ptr[i]);
    }
    puts("");
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

    //psuma =  funcion; // OJO esta mal, no coinciden en prototipo: la func y el ptr.
    //printf("\npsuma = %d", psuma(3,4));
}


int asc(double  a, double b){
    return a > b;
}


int desc(double a, double b){
    return a < b;
}

void ordenacionBurbuja(double *array, int size, int (*crit)(double, double)){
	// burbuja
	int i;
	int j;
	double aux_elem;
	
	for (i = 0; i < size - 1; i++){
	
		for (j = i+1; j < size; j++){
		
			if (crit(array[i], array[j])){
			
				aux_elem = array[i];
				array[i] = array[j];
				array[j] = aux_elem;
			}
		}
	}
}

void ordenarConCriterio(){
    double numeros[] = {5.7, 8.88, -9.9, 0.0, 123.6};

    imprimir(numeros, 5);

    ordenacionBurbuja(numeros, 5, asc);
    imprimir(numeros, 5);

    ordenacionBurbuja(numeros, 5, desc);
    imprimir(numeros, 5);
}


int main(){
    //definirPtrFuncion();
    //ordenarArray_qsort();

    //asignarPtrFunc_mal();
    ordenarConCriterio();

    puts("");
    return 0;
}