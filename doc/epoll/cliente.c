#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

static int send_all(
    int socket_fd,
    const void *buffer,
    size_t length)
{
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        ssize_t sent = send(
            socket_fd,
            cursor + total_sent,
            length - total_sent,
            0
        );

        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (sent == 0) {
            return -1;
        }

        total_sent += (size_t)sent;
    }

    return 0;
}

static int receive_response(
    int socket_fd,
    char *buffer,
    size_t buffer_size)
{
    while (1) {
        ssize_t received = recv(
            socket_fd,
            buffer,
            buffer_size - 1,
            0
        );

        if (received < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("recv");
            return -1;
        }

        if (received == 0) {
            puts(
                "El servidor ha cerrado la conexión."
            );

            return 0;
        }

        buffer[received] = '\0';

        printf(
            "Respuesta del servidor: %s",
            buffer
        );

        /*
         * El servidor envía una única respuesta
         * por cada mensaje del cliente.
         */
        return 1;
    }
}

int main(void)
{
    int socket_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address;

    memset(
        &server_address,
        0,
        sizeof(server_address)
    );

    server_address.sin_family = AF_INET;
    server_address.sin_port =
        htons(SERVER_PORT);

    int conversion_result = inet_pton(
        AF_INET,
        SERVER_IP,
        &server_address.sin_addr
    );

    if (conversion_result == 0) {
        fprintf(
            stderr,
            "Dirección IPv4 no válida: %s\n",
            SERVER_IP
        );

        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (conversion_result < 0) {
        perror("inet_pton");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (connect(
            socket_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) < 0) {
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf(
        "Conectado al servidor %s:%d\n",
        SERVER_IP,
        SERVER_PORT
    );

    puts(
        "Escribe mensajes para enviarlos."
    );

    puts(
        "Escribe salir para cerrar el cliente."
    );

    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        printf("\nMensaje: ");
        fflush(stdout);

        if (fgets(
                message,
                sizeof(message),
                stdin) == NULL) {
            if (ferror(stdin)) {
                perror("fgets");
            }

            break;
        }

        size_t message_length =
            strlen(message);

        /*
         * Eliminamos el salto de línea introducido
         * por fgets(), si está presente.
         */
        if (message_length > 0 &&
            message[message_length - 1] == '\n') {
            message[--message_length] = '\0';
        }

        if (strcmp(message, "salir") == 0) {
            puts("Cerrando el cliente...");
            break;
        }

        if (message_length == 0) {
            puts("No se enviará un mensaje vacío.");
            continue;
        }

        /*
         * Se envía también un salto de línea para que
         * la salida del servidor sea más legible.
         */
        message[message_length] = '\n';
        message_length++;

        if (send_all(
                socket_fd,
                message,
                message_length) < 0) {
            perror("send_all");
            close(socket_fd);
            return EXIT_FAILURE;
        }

        int receive_result = receive_response(
            socket_fd,
            response,
            sizeof(response)
        );

        if (receive_result < 0) {
            close(socket_fd);
            return EXIT_FAILURE;
        }

        if (receive_result == 0) {
            /*
             * El servidor cerró la conexión.
             */
            break;
        }
    }

    /*
     * Indicamos que el cliente no enviará más datos.
     * El servidor recibirá 0 en recv().
     */
    if (shutdown(socket_fd, SHUT_WR) < 0) {
        perror("shutdown");
    }

    if (close(socket_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
