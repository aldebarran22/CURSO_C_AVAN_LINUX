
#include <unistd.h>
#include <stdio.h>

int main(){

    // Tambien vale el comando: nproc  en la Shell!

    long num_proc = sysconf(_SC_NPROCESSORS_ONLN);
    printf("El numero de procesadores logicos: %ld\n", num_proc);
    return 0;
}
