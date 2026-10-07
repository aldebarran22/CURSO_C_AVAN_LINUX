/* Maquina de estados */

#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef state_t (*state_handler_t)(void);

// funciones de los distintos estados:
state_t handle_idle(void)
{
    puts("IDLE: Pasando a Running");
    return STATE_RUNNING;
}

state_t handle_running(void)
{
    puts("RUNNING: Pasando a Idle");
    return STATE_IDLE;
}

state_t handle_error(void)
{
    puts("ERROR: Pasando a Idle");
    return STATE_IDLE;
}

int main(void)
{
    state_handler_t handlers[STATE_COUNT] = 
    {
        handle_idle,
        handle_running,
        handle_error
    };

    // El estado inicial:
    state_t st = STATE_IDLE;

    for (int i = 0 ; i < 5 ; i++)
    {
        // Ejecuta la logica del estado y obtiene el siguiente:
        st = handlers[st](); 
    }
    return 0;
}