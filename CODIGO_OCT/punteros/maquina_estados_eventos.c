

// Definicion de una maquina de estados con eventos
// Dado un estado, recibimos un eventos y transitamos
// a un estado u otro.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Los estados:
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;


// Los eventos:
typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_FAIL,
    EVENT_COUNT
} event_t;

// La maquina de estados:
typedef struct {
    state_t state; // El estado actual
    // mas campos ...
    // informacion de contexto.
} fsm_t;


// Las funciones manejadoras reciben la maq. de estados
// y el evento, en funcion del evento hace la transacion
// a un estado u otro!
typedef state_t (*state_handle_t)(fsm_t *, event_t);

// Una funcion para cada estado ...

state_t handle_idle(fsm_t *fsm, event_t event){
    
    switch (event){
        case EVENT_START:
            puts("En IDLE con START pasamos a RUNNING");
            return STATE_RUNNING;
        
        default:
            puts("IDLE: con otro evento");
            return STATE_IDLE;
    }

}


state_t handle_running(fsm_t *fsm, event_t event){
      switch (event){
        case EVENT_STOP:
            puts("En RUNNING con STOP pasamos a IDLE");
            return STATE_IDLE;

        case EVENT_FAIL:
            puts("En RUNNING con FAIL pasamos a ERROR");
            return STATE_ERROR;
        
        default:
            puts("RUNNING: con otro evento");
            return STATE_RUNNING;
    }
}

state_t handle_error(fsm_t *fsm, event_t event){

     switch (event){
        case EVENT_START:
            puts("En ERROR con START pasamos a RUNNING");
            return STATE_RUNNING;
        
        default:
            puts("ERROR: con otro evento");
            return STATE_ERROR;
    }
}


event_t generarEvento(){
    /*
    event_t eventos[EVENT_COUNT] = 
    {
        EVENT_START, 
        EVENT_STOP,
        EVENT_FAIL
    };
    */
    int pos = rand() % EVENT_COUNT;
    return pos;
    //return eventos[pos];
}




int main(){
    // Inicializar semilla: num aleatorios
    srand(time(NULL));

    /*
    // Definir un arrays con todas las funciones manejadoras
    state_handle_t handlers[STATE_COUNT] = {
        handle_idle, handle_running, handle_error
    };

    // Definir el estado inicial:
    state_t st = STATE_ERROR;

    for (int i = 0 ; i < 5 ; i++){
        st = handlers[st]();
    }
    */
    puts("");
    return 0;
}