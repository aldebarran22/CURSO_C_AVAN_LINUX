

// Definicion de una maquina de estados
// Estados, eventos y transiciones

#include <stdio.h>

// Definicion de estados
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;


// DEfinicion de eventos:
typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_FAIL,
    EVENT_COUNT
} event_t;

// La maquina de estados:
typedef struct {
    state_t state; // Estado actual
    // mas informacion ...

} fms_t;



typedef state_t (*state_handle_t)(fms_t *, event_t);

// Una funcion para cada estado ...

state_t handle_idle(fms_t * fms, event_t ev){
    // logica de este estado ...

    switch (ev){
        case EVENT_START:
            puts("\nEn IDLE recibe EV START");
            return STATE_RUNNING;

        default:
            // Cualquier otro estado no produce transicion;
            return STATE_IDLE;
    }
}


state_t handle_running(fms_t * fms, event_t ev){
    // logica de este estado ...
    switch (ev){
        case EVENT_STOP:
            puts("\nEn RUNNING recive EV STOP");
            return STATE_IDLE;

        case EVENT_FAIL:
            puts("\nEn RUNNING recive EV FAIL");
            return STATE_ERROR;

        default:
            puts("\nEn RUNNING recive EV START");
            return STATE_RUNNING;
    }

}

state_t handle_error(fms_t * fms, event_t ev){
    switch (ev){
        case EVENT_START:
            puts("\nEn ERROR recive EV START");
            return STATE_RUNNING;

        default:
            puts("\nEn ERROR con otro evento -> ERROR");
            return STATE_ERROR;
    }
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