#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"

#define PORT 8080

int send_all(int fd, const void *buffer, size_t length);

int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = 0
    };

    int result = inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_address.sin_addr
    );

    if (result != 1) {
        fprintf(stderr, "Dirección IP no válida\n");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    result = connect(
        socket_fd,
        (const struct sockaddr *) &server_address,
        sizeof(server_address)
    );

    if (result < 0) {
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    message_header_t header = {
        .version = 1,
        .type = 2,
        .length = 128,
        .identifier = 1000
    };

    uint8_t buffer[HEADER_SIZE];

    serialize_header(buffer, &header);

    result = send_all(
        socket_fd,
        buffer,
        sizeof(buffer)
    );

    if (result < 0) {
        perror("send_all");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf(
        "Cabecera enviada correctamente: %zu bytes\n",
        sizeof(buffer)
    );

    close(socket_fd);

    return EXIT_SUCCESS;
}
