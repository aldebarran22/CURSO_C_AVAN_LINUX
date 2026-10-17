// Ejemplo de un callback

#include <stdio.h>

// Definir un tipo de puntero a funcion: void funcion(int);
typedef void (*callback_t)(int result);


void do_work(callback_t cb){
    // Recibe por parametro  el callback, la funcion 
    // que tiene que ejecutar.
    int r = 42;
    cb(r);      // Invocamos el callback: on_done
}


void on_done(int result){
    printf("\nResultado del callback: %d", result);
}

int main(){
    do_work(on_done);
    puts("");
    return 0;
}