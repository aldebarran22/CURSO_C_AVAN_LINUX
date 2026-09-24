#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10
#define MAX_LINEA 256

void testCalloc(){
	int i;
	int *p = (int *) calloc(N, sizeof(int));
	
	for (i = 0 ; i < N ; i++)
		printf("%d ", p[i]);
	puts("");
	
	free(p);
	p = NULL;
}


void testRealloc(){
	FILE *f;
	char linea[MAX_LINEA];
	int n, i;
	char **ptr = NULL;	
	int total = 0;
	
	if ((f = fopen("calloc_realloc.c","rt"))!=NULL){
		
		while (fgets(linea, MAX_LINEA, f)){
			n = strlen(linea);
			linea[n-1] = '\0';
			
			if (n-1 >= 1){
				ptr = (char **) realloc(ptr, (total+1) * sizeof(char *));
				ptr[total] = (char *) calloc(n, sizeof(char)); // n ya incluye strlen(linea)+\0
				strcpy(ptr[total], linea);
				total++;
				
				//printf("%s %d\n", linea, strlen(linea));
			}
				
		}
		
		fclose(f);
	}
	printf("Lineas procesadas: %d\n", total);
	
	// Imprimir las lineas:
	for (i = 0 ; i < total ; i++){
		puts(ptr[i]);
		
		free(ptr[i]);
		ptr[i] = NULL;
	}
		
	free(ptr);
	ptr = NULL;		
	
}


int main(){
	//testCalloc();
	testRealloc();
	return 0;
}
