

// Definicion de una maquina de estados
// Estados y transiciones

#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;



typedef state_t (*state_handle_t)(void);

// Una funcion para cada estado ...
state_t handle_idle(void){
    // logica de este estado ...
    puts("IDLE: pasando a RUNNING");

    // Al siguiente estado que transita!
    return STATE_RUNNING;
}


state_t handle_running(void){
    // logica de este estado ...
    puts("RUNNING: pasando a IDLE");

    // Al siguiente estado que transita!
    return STATE_IDLE;
}

state_t handle_error(void){
    // logica de este estado ...
    puts("ERROR: pasando a IDLE");

    // Al siguiente estado que transita!
    return STATE_IDLE;
}


int main(){

    // Definir un arrays con todas las funciones manejadoras
    state_handle_t handlers[STATE_COUNT] = {
        handle_idle, handle_running, handle_error
    };

    // Definir el estado inicial:
    state_t st = STATE_ERROR;

    for (int i = 0 ; i < 5 ; i++){
        st = handlers[st]();
    }

    puts("");
    return 0;
}