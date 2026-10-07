// Callback con contexto:

#include <stdio.h>


typedef void (*callback_t)(int result, void* user_data);


typedef  struct {
    const char* tag;
} context_t;


void do_work(callback_t cb, void* user_data){
    // Recibe por parametro  el callback, la funcion 
    // que tiene que ejecutar.
    int r = 42;
    cb(r, user_data);      // Invocamos el callback: on_done
}


void on_done(int result, void* user_data){
    // El puntero void se adapta a context_t*
    context_t* ctx = user_data;
    printf("[%s] Resultado: %d", ctx->tag, result);
}

int main(){
    context_t ctx = {.tag = "TASK1"};
    do_work(on_done, &ctx);
    puts("");
    return 0;
}



