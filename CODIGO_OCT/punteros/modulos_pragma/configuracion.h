#pragma once

// Variable definida en configuracion.C
// Si se declara con extern permite acceder
// a ella desde otros modulos

extern int incremento_configurado;

// Funciones publicas del modulo:
void configuracion_establecer_incremento(int incremento);
void configuracion_aplicar_incremento(void);
void configuracion_mostrar();
