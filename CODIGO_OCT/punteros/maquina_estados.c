

// Definicion de una maquina de estados
// Estados y transiciones

#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;




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


    puts("");
    return 0;
}