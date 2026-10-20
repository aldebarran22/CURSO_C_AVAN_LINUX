// Errores relacionados con la memoria dinamica:

#include <stdlib.h>
#include <stdio.h>


int memoryleaks_file_mal(const char *path){
    FILE *f;
    char *buffer = malloc(100 * sizeof(char));
    
    if (buffer == NULL){
        return EXIT_FAILURE;
    }


    if ((f = fopen(path, "rt"))== NULL){
        return EXIT_FAILURE;
    }

    free(buffer);
    buffer = NULL;
    return EXIT_SUCCESS;
}


int main(){
    memoryleaks_file("pruebas.txt");
    return 0;
}