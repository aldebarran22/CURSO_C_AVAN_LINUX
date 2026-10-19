

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
    // Generar un evento aleatorio!
    return rand() % EVENT_COUNT;  
}


int main(){
    // Inicializar semilla: num aleatorios
    srand(time(NULL));

    event_t ev; // El evento que se produce ...

    // Las funciones manejadoras:
    state_handle_t handlers[STATE_COUNT] = {
        handle_idle, handle_running, handle_error
    };

    // La maquina  con el estado inicial:
    fsm_t fsm = {
        .state = STATE_IDLE
    };

    // Bucle de transiciones hasta que se produzca un error!
    while (fsm.state != STATE_ERROR){
        ev = generarEvento();
        fsm.state = handlers[fsm.state](&fsm, ev);
    }

    puts("");
    return 0;
}