
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
    if (lineas == NULL){
        perror("malloc");
        fclose(f);
        return;
    }

    while (fgets(linea, sizeof(linea), f)){
        size_t longitud = strlen(linea);
        if (longitud > 0 && linea[longitud-1]=='\n'){
            linea[--longitud] = '\0';
        }
        //puts(linea);

        // Comprobar si hay que ampliar el bloque:
        if (total == capacidad){
            // El nuevo tamaño:
            size_t nueva_capacidad = capacidad * 2;

            char **aux = realloc(lineas, nueva_capacidad);
            if (aux == NULL){
                perror("realloc");
                break;
            }

            // Si hay ido bien la reserva:
            lineas = aux;
            capacidad = nueva_capacidad;
        }

        // Reservar para la linea leida: +1 es por el \0
        lineas[total] = malloc((longitud+1) * sizeof(char));
        if (lineas[total] == NULL){
            perror("malloc");
            break;
        }

        memcpy(lineas[total], linea, longitud+1);

        total++; // lleva el numero de lineas procesadas del fichero!
    }

    fclose(f);
}


int main(){
    cargaFichero("./pedidos_final.csv");
    puts("");
    return 0;
}