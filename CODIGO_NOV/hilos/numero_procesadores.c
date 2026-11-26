// Obtener el n. de procesadores lógicos de la máquina

#include <stdio.h>
#include <unistd.h>

int main(){
    // El comando de la shell: nproc

    long  num_proc = sysconf(_SC_NPROCESSORS_ONLN);
    printf("\nNumero procesadores logicos: %ld\n", num_proc);
    return 0;
}