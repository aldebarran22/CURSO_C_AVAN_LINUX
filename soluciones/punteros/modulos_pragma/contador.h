
#pragma once

// Declaracion externa, no se reserva memoria
// Se indica que la variable esta definida en
// otro fichero(en el .C)

extern int contador_global;

// Funciones publicas, no es necesario
// definirlas con extern, ya lo tienen por  defecto

void contador_incrementar(void);
void contador_mostrar(void);
int contador_obtener_operaciones();