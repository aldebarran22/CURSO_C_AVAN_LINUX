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


int sumar(int a, int b){
	return a+b;
}

int restar(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}

void imprimir(double *array, int n){
	int i;
		
	for (i=0; i <  n; i++)
		printf("%g ", array[i]);
	puts("");
	
}


