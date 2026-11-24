
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cargaFichero(const char *path){
    FILE *f = fopen(path, "rt");

    if (f == NULL){
        perror("fopen");
        return;
    }

    char linea[1024];
    while (fgets(linea, sizeof(linea), f)){
        size_t longitud = strlen(linea);
        if (longitud > 0 && linea[longitud-1]=='\n'){
            linea[--longitud] = '\0';
        }
        puts(linea);
    }

    fclose(f);
}


int main(){
    cargaFichero("./pedidos_final.csv");
    puts("");
    return 0;
}