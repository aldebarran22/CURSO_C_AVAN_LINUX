// Implementacion del patron STATE

#include <stdio.h>


// Nombre de los estados:
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_id_t;


// Nombre de los eventos:
typedef  enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_FAILURE,
    EVENT_RESET
} event_t;


// Declaracion forward:
typedef struct state_machine state_machine_t;


// Estructura para un estado:
typedef struct {
    state_id_t id;
    void (*on_enter)(state_machine_t* machine);
    void (*on_event)(state_machine_t* machine);
    void (*on_exit)(state_machine_t* machine);
} state_t;


// Estado de la maquina:
struct state_machine {
    const state_t *current_state;
    int error_code;
    unsigned int processed_events;
};


// Funciones:
static void state_machine_transition(state_machine_t *machine, const state_t *next_state);


static void idle_enter(state_machine_t *machine);
static void idle_event(state_machine_t *machine, event_t event);
static void idle_exit(state_machine_t *machine);


static void running_enter(state_machine_t *machine);
static void running_event(state_machine_t *machine, event_t event);
static void running_exit(state_machine_t *machine);


static void error_enter(state_machine_t *machine);
static void error_event(state_machine_t *machine, event_t event);
static void error_exit(state_machine_t *machine);


// Definicion de los estados:
static const state_t idle_state = {
    .id = STATE_IDLE,
    .on_enter = idle_enter,
    .on_event = idle_event,
    .on_exit = idle_exit
};


static const state_t running_state = {
    .id = STATE_RUNNING,
    .on_enter = running_enter,
    .on_event = running_event,
    .on_exit = running_exit
};


static const state_t error_state = {
    .id = STATE_ERROR,
    .on_enter = error_enter,
    .on_event = error_event,
    .on_exit = error_exit
};



static void state_machine_transition(state_machine_t *machine, const state_t *next_state){

}



static void idle_enter(state_machine_t *machine){

}

static void idle_event(state_machine_t *machine, event_t event){

}

static void idle_exit(state_machine_t *machine){

}


static void running_enter(state_machine_t *machine){

}

static void running_event(state_machine_t *machine, event_t event){

}

static void running_exit(state_machine_t *machine){

}


static void error_enter(state_machine_t *machine){

}

static void error_event(state_machine_t *machine, event_t event){

}

static void error_exit(state_machine_t *machine){

}

int main(){

    return 0;
}