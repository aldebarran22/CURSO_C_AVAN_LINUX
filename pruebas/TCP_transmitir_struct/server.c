#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"

#define PORT 8080

int recv_all(int fd, void *buffer, size_t length);

static void print_message_header(const message_header_t *header)
{
    printf("message_header_t\n");
    printf("Version: %u\n", (unsigned)header->version);
    printf("Type: %u\n", (unsigned)header->type);
    printf("Length: %u\n", (unsigned)header->length);
    printf("Identifier: %u\n", (unsigned)header->identifier);
}

int main(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int reuse = 1;

    if (setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &reuse,
            sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if (bind(
            server_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);

    int client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    uint8_t buffer[HEADER_SIZE];

    int result = recv_all(
        client_fd,
        buffer,
        sizeof(buffer)
    );

    if (result == 0) {
        fprintf(stderr, "El cliente cerró la conexión\n");
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (result < 0) {
        perror("recv");
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    message_header_t header;

    deserialize_header(&header, buffer);
    print_message_header(&header);

    close(client_fd);
    close(server_fd);

    return EXIT_SUCCESS;
}
