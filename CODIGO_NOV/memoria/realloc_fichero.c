
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 8

void cargaFichero(const char *path){
    FILE *f = fopen(path, "rt");

    if (f == NULL){
        perror("fopen");
        return;
    }

    char **lineas;
    size_t total = 0;
    size_t capacidad = CAPACIDAD_INICIAL;
    char linea[1024];

    // Reserva inicial para 8 filas!
    lineas = malloc(capacidad * sizeof(char *));

    while (fgets(linea, sizeof(linea), f)){
        size_t longitud = strlen(linea);
        if (longitud > 0 && linea[longitud-1]=='\n'){
            linea[--longitud] = '\0';
        }
        //puts(linea);

        
    }

    fclose(f);
}


int main(){
    cargaFichero("./pedidos_final.csv");
    puts("");
    return 0;
}