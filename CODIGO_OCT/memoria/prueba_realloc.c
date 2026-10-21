// Prueba de  realloc para leer / cargar un fichero:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 1024
#define CAPACIDAD_INICIAL 8

void cargaFichero(const char *path){

    FILE *f = fopen(path, "r");

    if (f == NULL){
        perror("fopen");
        return;
    }

    char linea[MAX_LINEA];
    size_t total = 0;
    size_t capacidad = CAPACIDAD_INICIAL;

    // Reservamos con la capacidad inicial:
    char **lineas = malloc(capacidad * sizeof(*lineas)); // sizeof(char *)

    if (lineas == NULL){
        perror("malloc");
        fclose(f);
        return;
    }

    while (fgets(linea, sizeof(linea), f)){
        size_t longitud = strlen(linea);

        if (longitud > 0 && linea[longitud-1]=='\n'){
            linea[--longitud] = '\0'; // machacar el \n del final de la linea
        }

        if (longitud == 0){
            continue;
        }

        puts(linea);
    }

    free(lineas);
    lineas = NULL;
    fclose(f);

}


int main(){
    cargaFichero("pedidos_final.csv");
    return 0;
}