/* Maquina de estados con eventos */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;


typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_FAIL,
    EVENT_COUNT
} event_t;


typedef struct {
    state_t state;      // El estado actual
    int fd;             // ejemplo: socket asociado
    void *user_data;    // opcional
} fsm_t;



typedef state_t (*state_handler_t)(fsm_t*, event_t);



// funciones de los distintos estados:
state_t idle_on_event(fsm_t *fsm, event_t ev)
{
    switch (ev)
    {
    case EVENT_START:
        puts("IDLE con START: Pasando a Running");
        return STATE_RUNNING;
    
    default:
        puts("IDLE con otro evento");
        return STATE_IDLE;
    }        
}


state_t running_on_event(fsm_t *fsm, event_t ev)
{
    switch (ev)
    {
    case EVENT_STOP:
        puts("RUNNING con STOP: Pasando a Idle");
        return STATE_IDLE;
    
    case EVENT_FAIL:
        puts("RUNNING con FAIL: Pasando a Error");
        return STATE_ERROR;

    default:
        puts("RUNNING con otro evento: Pasando a Running");
        return STATE_RUNNING;
    }        
}


state_t error_on_event(fsm_t *fsm, event_t ev)
{
    switch (ev)
    {
    case EVENT_START:
        puts("ERROR con START: Pasando a Running");
        return STATE_RUNNING;
    
    default:
        puts("ERROR con otro evento: Pasando a Error");
        return STATE_ERROR;
    }        
}



// Tabla de handlers:
state_handler_t handlers[STATE_COUNT] = {
    idle_on_event,
    running_on_event,
    error_on_event
};



void fsm_dispatch(fsm_t *fsm, event_t ev)
{
    // Lanzar el siguiente estado a partir del evento:
    fsm->state = handlers[fsm->state](fsm, ev);
}


event_t obtener_evento(){
    int pos = rand() % EVENT_COUNT;
    switch (pos)
    {
    case 0:
        return STATE_IDLE;

    case 1:
        return STATE_RUNNING;

    
    default:
        return STATE_ERROR;
    }
}


int main(void)
{
    srand(time(NULL));
    event_t ev;
   
    // El estado inicial: (inactivo)
    fsm_t fsm = { .state = STATE_IDLE};

    while (fsm.state != STATE_ERROR){
        ev = obtener_evento();
        fsm_dispatch(&fsm, ev);
    }
   
    return 0;
}