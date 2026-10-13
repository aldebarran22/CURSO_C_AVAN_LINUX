// Servidor basico de 1 a 1:
// Solo se conecta un cliente

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

static int send_all(int socket_fd,  const void *buffer, size_t length) {
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        // Entero con signo, por si devuelve -1
        ssize_t sent = send(socket_fd, cursor + total_sent, length - total_sent, 0);

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
    int server_fd = -1;
    int client_fd = -1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Reutilizar la direccion:
    int reuse_address = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Direccion del servidor, configurar propiedades:
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Solo un cliente en cola: backlog
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Servidor escuchando en el puerto %d\n", SERVER_PORT);

    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    char client_ip[INET_ADDRSTRLEN];

    // Convierte la dir. IP del cliente de binario a texto:
    if (inet_ntop( AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
        strcpy(client_ip, "desconocida");
    }

    printf("Cliente conectado desde %s:%u\n", client_ip, (unsigned int)ntohs(client_address.sin_port));

    char buffer[BUFFER_SIZE];

    ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (received < 0) {
        perror("recv");
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (received == 0) {
        puts("El cliente cerró la conexión sin enviar datos.");

    } else {
        buffer[received] = '\0';

        printf("Mensaje recibido: %s\n", buffer);

        const char response[] =
            "Mensaje recibido correctamente";

        if (send_all(client_fd, response, sizeof(response) - 1) < 0) {
            perror("send");
            close(client_fd);
            close(server_fd);
            return EXIT_FAILURE;
        }
    }

    close(client_fd);
    close(server_fd);

    return EXIT_SUCCESS;
}