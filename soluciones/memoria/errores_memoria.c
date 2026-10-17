#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ERROR 1:
 * Liberar dos veces el mismo bloque.
 */
static void test_double_free(void)
{
    int *numbers = malloc(
        10 * sizeof(*numbers)
    );

    if (numbers == NULL) {
        perror("malloc");
        return;
    }

    free(numbers);

    /*
     * ERROR:
     * numbers sigue conteniendo la dirección
     * del bloque que ya se ha liberado.
     */
    free(numbers);
}

/*
 * ERROR 2:
 * Acceder a un bloque después de liberarlo.
 */
static void test_use_after_free(void)
{
    int *number = malloc(sizeof(*number));

    if (number == NULL) {
        perror("malloc");
        return;
    }

    *number = 100;

    free(number);

    /*
     * ERROR:
     * Se accede a memoria que ya no pertenece
     * al programa.
     */
    printf("Valor: %d\n", *number);
}

/*
 * ERROR 3:
 * Devolver la dirección de una variable local.
 */
static int *get_invalid_pointer(void)
{
    int local_number = 50;

    /*
     * ERROR:
     * local_number deja de existir cuando
     * termina esta función.
     */
    return &local_number;
}

static void test_local_variable_address(void)
{
    int *pointer = get_invalid_pointer();

    /*
     * ERROR:
     * pointer ya no apunta a un objeto válido.
     */
    printf("Valor local: %d\n", *pointer);
}

/*
 * ERROR 4:
 * Escribir fuera de los límites del bloque.
 */
static void test_heap_buffer_overflow(void)
{
    int *numbers = malloc(
        5 * sizeof(*numbers)
    );

    if (numbers == NULL) {
        perror("malloc");
        return;
    }

    /*
     * El array solamente tiene posiciones:
     *
     * 0, 1, 2, 3 y 4
     */
    numbers[5] = 100;

    free(numbers);
}

/*
 * ERROR 5:
 * Leer antes del comienzo del bloque.
 */
static void test_heap_buffer_underflow(void)
{
    int *numbers = malloc(
        5 * sizeof(*numbers)
    );

    if (numbers == NULL) {
        perror("malloc");
        return;
    }

    /*
     * ERROR:
     * La posición -1 está antes del bloque.
     */
    printf("Valor: %d\n", numbers[-1]);

    free(numbers);
}

/*
 * ERROR 6:
 * Fuga de memoria.
 */
static void test_memory_leak(void)
{
    char *message = malloc(100);

    if (message == NULL) {
        perror("malloc");
        return;
    }

    strcpy(
        message,
        "Este bloque no se libera"
    );

    puts(message);

    /*
     * ERROR:
     * Falta free(message).
     */
}

/*
 * ERROR 7:
 * Perder el bloque original si realloc falla.
 */
static void test_unsafe_realloc(void)
{
    int *numbers = malloc(
        10 * sizeof(*numbers)
    );

    if (numbers == NULL) {
        perror("malloc");
        return;
    }

    /*
     * ERROR:
     * Si realloc devuelve NULL, numbers pasa
     * a valer NULL y se pierde la dirección
     * del bloque original.
     */
    numbers = realloc(
        numbers,
        1000000000ULL * sizeof(*numbers)
    );

    if (numbers == NULL) {
        puts("realloc ha fallado");

        /*
         * Ya no podemos liberar el bloque original,
         * porque hemos perdido su dirección.
         */
        return;
    }

    free(numbers);
}

/*
 * ERROR 8:
 * Copiar una cadena en un bloque demasiado pequeño.
 */
static void test_string_overflow(void)
{
    char *text = malloc(5);

    if (text == NULL) {
        perror("malloc");
        return;
    }

    /*
     * ERROR:
     * "Hola mundo" necesita 11 bytes:
     *
     * 10 caracteres y el terminador nulo.
     */
    strcpy(text, "Hola mundo");

    puts(text);

    free(text);
}

/*
 * ERROR 9:
 * Liberar memoria que no fue reservada
 * mediante malloc, calloc o realloc.
 */
static void test_invalid_free(void)
{
    int local_number = 25;
    int *pointer = &local_number;

    /*
     * ERROR:
     * pointer apunta a una variable automática
     * almacenada normalmente en la pila.
     */
    free(pointer);
}

/*
 * ERROR 10:
 * Utilizar una parte no inicializada de un bloque.
 */
static void test_uninitialized_memory(void)
{
    int *numbers = malloc(
        5 * sizeof(*numbers)
    );

    if (numbers == NULL) {
        perror("malloc");
        return;
    }

    numbers[0] = 10;

    /*
     * ERROR:
     * numbers[1] no se ha inicializado.
     */
    printf(
        "Valor no inicializado: %d\n",
        numbers[1]
    );

    free(numbers);
}

int main(void)
{
    /*
     * Activa solamente una prueba cada vez.
     */

    test_double_free();

    /*
    test_use_after_free();
    test_local_variable_address();
    test_heap_buffer_overflow();
    test_heap_buffer_underflow();
    test_memory_leak();
    test_unsafe_realloc();
    test_string_overflow();
    test_invalid_free();
    test_uninitialized_memory();
    */

    return EXIT_SUCCESS;
}