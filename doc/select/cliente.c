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
            MSG_NOSIGNAL
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
    server_address.sin_port = htons(SERVER_PORT);

    int conversion_result = inet_pton(
        AF_INET,
        SERVER_IP,
        &server_address.sin_addr
    );

    if (conversion_result == 0) {
        fprintf(
            stderr,
            "Dirección IP no válida\n"
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

    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        printf("Mensaje: ");

        if (fgets(
                message,
                sizeof(message),
                stdin) == NULL) {
            break;
        }

        size_t length = strlen(message);

        if (length > 0 &&
            message[length - 1] == '\n') {
            message[--length] = '\0';
        }

        if (strcmp(message, "salir") == 0) {
            break;
        }

        if (length == 0) {
            continue;
        }

        if (send_all(
                socket_fd,
                message,
                length) < 0) {
            perror("send_all");
            close(socket_fd);
            return EXIT_FAILURE;
        }

        ssize_t received = recv(
            socket_fd,
            response,
            sizeof(response) - 1,
            0
        );

        if (received < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("recv");
            close(socket_fd);
            return EXIT_FAILURE;
        }

        if (received == 0) {
            puts("El servidor cerró la conexión");
            break;
        }

        response[received] = '\0';

        printf(
            "Eco del servidor: %s\n",
            response
        );
    }

    close(socket_fd);

    return EXIT_SUCCESS;
}
