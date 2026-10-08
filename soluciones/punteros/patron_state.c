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
    void (*on_event)(state_machine_t* machine, event_t event);
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

    if (machine == NULL || next_state == NULL){
        return;
    }

    // Ejecuta  on_exit del estado anterior:
    if (machine->current_state != NULL &&
        machine->current_state->on_exit != NULL){
            machine->current_state->on_exit(machine);
    }

    // Cambiar al estado actual al siguiente:
    machine->current_state = next_state;

    // Ejecutar on_enter del nuevo estado;
    if (machine->current_state->on_enter != NULL){
        machine->current_state->on_enter(machine);
    }
}


// EVENTOS DE IDLE:----------------------------------------------------------
static void idle_enter(state_machine_t *machine){

    (void) machine;
    puts("Entrado en IDLE");
}

static void idle_event(state_machine_t *machine, event_t event){

    if (event == EVENT_START){
        state_machine_transition(machine, &running_state);
    }
}

static void idle_exit(state_machine_t *machine){

    (void) machine;
    puts("Saliendo de IDLE");
}


// EVENTOS DE RUNNING:---------------------------------------------------------
static void running_enter(state_machine_t *machine){

    (void) machine;
    puts("Entrado en RUNNING");
}

static void running_event(state_machine_t *machine, event_t event){

    if (event == EVENT_STOP){
        state_machine_transition(machine, &idle_state);

    } else if (event == EVENT_FAILURE){
        machine->error_code = 1;

        state_machine_transition(machine, &error_state);
    }
}

static void running_exit(state_machine_t *machine){

    (void) machine;
    puts("Saliendo de RUNNING");
}


// EVENTOS DEL ESTADO ERROR:-------------------------------------------------------
static void error_enter(state_machine_t *machine){

    printf("Entrando en ERROR. Codigo: %d\n", machine->error_code);
}

static void error_event(state_machine_t *machine, event_t event){

    if (event == EVENT_RESET){
        machine->error_code = 0;

        state_machine_transition(machine, &idle_state);
    }
}

static void error_exit(state_machine_t *machine){

    (void) machine;
    puts("Saliendo de ERROR");
}


// Despacho de eventos:
static void state_machine_dispatch(state_machine_t *machine, event_t event){

    if (machine == NULL ||
        machine->current_state == NULL ||
        machine->current_state->on_event == NULL){
            return;
    }
    

    machine->processed_events++;
    machine->current_state->on_event(machine,  event);
}


int main(){

    // Estado inicial de la maquina:
    state_machine_t machine = {
        .current_state = NULL,
        .error_code = 0,
        .processed_events = 0
    };

    // Asignar el primer estado: IDLE
    state_machine_transition(&machine, &idle_state);

    // Generar eventos:
    state_machine_dispatch(&machine, EVENT_START);
    state_machine_dispatch(&machine, EVENT_FAILURE);
    state_machine_dispatch(&machine, EVENT_RESET);

    printf("Eventos procesados: %u\n", machine.processed_events);

    return 0;
}