#include <stdio.h>
#include <stdlib.h>

/*
gcc \
    -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g3 \
    -O0 \
    -fno-omit-frame-pointer \
    depurar.c \
    -o depurar


-g3
    Incluye información de depuración.

-O0
    Desactiva optimizaciones para que la ejecución se corresponda
    mejor con las líneas del código fuente.

-fno-omit-frame-pointer
    Facilita la inspección de la pila de llamadas.


PASOS:
gdb ./depurar

(gdb) break calcular_total o b calcular_total

Valido tambien: (gdb) break depurar.c:calcular_total

# consultar los breakpoint:
(gdb)info breakpoints o abreviado: "i b"

(gdb) run o una 'r'

# para mostrar el codigo cercano:
(gdb) list

o listar una funcion: list nombre_funcion

# consultar los parametros.
(gdb) info args

# consultar un param o variable
(gdb) print cantidad o "p cantidad"

Si usamos "next" avanza  hasta el siguiente breakpoint

posible secuencia:
(gdb) n
(gdb) p i
$5 = 0

(gdb) p valores[i]
$6 = 10

(gdb) n
(gdb) p total
$7 = 10

(gdb) n
(gdb) p i
$8 = 1

(gdb) p valores[i]
$9 = 20

(gdb) n
(gdb) p total
$10 = 30


# Para mostrar variables cada vez que se detenga:
display i
display total

# Ver variables locales:
info locals

# Ver la pila de llamadas:
backtrace o bt

# Terminar la funcion actual:
finish

# para salir de gdb: quit




*/

static int calcular_total(
    const int valores[],
    size_t cantidad)
{
    int total = 0;

    for (size_t i = 0; i < cantidad; i++) {
        total += valores[i];
    }

    return total;
}

int main(void)
{
    int numeros[] = {
        10,
        20,
        30,
        40
    };

    size_t cantidad =
        sizeof(numeros) / sizeof(numeros[0]);

    int resultado = calcular_total(
        numeros,
        cantidad
    );

    printf(
        "Resultado: %d\n",
        resultado
    );

    return EXIT_SUCCESS;
}
