#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10
#define MAX_LINEA 256


#define MAX_LINEA_2 1024
#define CAPACIDAD_INICIAL 8

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
	char **aux = NULL;
	int total = 0;
	
	if ((f = fopen("calloc_realloc.c","rt"))!=NULL){
		
		while (fgets(linea, MAX_LINEA, f)){
			n = strlen(linea);
			linea[n-1] = '\0';
			
			if (n-1 >= 1){
				// Esta linea es peligrosa si realloc devuelve NULL, usar un aux
				//ptr = (char **) realloc(ptr, (total+1) * sizeof(char *));

				// No es necesario el casting
				aux = realloc(ptr, (total+1) * sizeof(char *));

				if (aux != NULL){
					ptr =  aux;
					ptr[total] = calloc(n, sizeof(char)); // n ya incluye strlen(linea)+\0
					strcpy(ptr[total], linea);
					total++;		

				} else {
					break;
				}						
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
	puts("");
	
}




void testRealloc2(void){
    FILE *file = fopen("calloc_realloc.c", "r");

    if (file == NULL) {
        perror("fopen");
        return;
    }

    char linea[MAX_LINEA_2];

    size_t total = 0;
    size_t capacidad = CAPACIDAD_INICIAL;

    char **lineas = malloc(capacidad * sizeof(*lineas) );

    if (lineas == NULL) {
        perror("malloc");
        fclose(file);
        return;
    }

    while (fgets(linea, sizeof(linea), file) != NULL) {
        size_t longitud = strlen(linea);

        if (longitud > 0 && linea[longitud - 1] == '\n') {
            linea[--longitud] = '\0';
        }

        if (longitud == 0) {
            continue;
        }

        if (total == capacidad) {
            size_t nueva_capacidad = capacidad * 2;

            char **aux = realloc(lineas, nueva_capacidad * sizeof(*lineas));

            if (aux == NULL) {
                perror("realloc");
                break;
            }

            lineas = aux;
            capacidad = nueva_capacidad;
        }

        lineas[total] = malloc(longitud + 1);

        if (lineas[total] == NULL) {
            perror("malloc");
            break;
        }

		
        memcpy(lineas[total], linea, longitud + 1);

        total++;
    }

    if (ferror(file)) {
        perror("fgets");
    }

    fclose(file);
    printf("Líneas procesadas: %zu\n", total);

    for (size_t i = 0; i < total; i++) {
        puts(lineas[i]);
        free(lineas[i]);
    }

    free(lineas);
	lineas = NULL;
	puts("");
}


int main(){
	testCalloc();
	testRealloc2();
	return 0;
}
