#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define LISTEN_BACKLOG 10
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

static void close_all_clients(
    fd_set *master_set,
    int server_fd,
    int max_fd)
{
    for (int fd = 0; fd <= max_fd; fd++) {
        if (fd != server_fd &&
            FD_ISSET(fd, master_set)) {
            close(fd);
        }
    }
}

int main(void)
{
    int server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int reuse_address = 1;

    if (setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &reuse_address,
            sizeof(reuse_address)) < 0) {
        perror("setsockopt");
        close(server_fd);
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
    server_address.sin_addr.s_addr =
        htonl(INADDR_ANY);

    if (bind(
            server_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(
            server_fd,
            LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /*
     * select() únicamente puede trabajar con
     * descriptores menores que FD_SETSIZE.
     */
    if (server_fd >= FD_SETSIZE) {
        fprintf(
            stderr,
            "El descriptor del servidor supera FD_SETSIZE\n"
        );

        close(server_fd);
        return EXIT_FAILURE;
    }

    /*
     * master_set contiene todos los descriptores
     * que queremos vigilar permanentemente.
     */
    fd_set master_set;

    // Inicializar la lista de sockets
    FD_ZERO(&master_set);

    // Añade el descriptor del socket del servidor a la lista:
    FD_SET(server_fd, &master_set);

    /*
     * max_fd es el descriptor más alto contenido
     * actualmente en master_set.
     */
    int max_fd = server_fd;

    printf(
        "Servidor select escuchando en el puerto %d\n",
        SERVER_PORT
    );

    while (1) {
        /*
         * select() modifica el conjunto recibido,
         * dejando solamente los descriptores listos.
         *
         * Por ese motivo hacemos una copia del
         * conjunto maestro en cada iteración.
         */
        fd_set read_set = master_set;

        int ready = select(
            max_fd + 1,
            &read_set,
            NULL,
            NULL,
            NULL
        );

        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("select");
            break;
        }

        /*
         * Se recorren todos los posibles descriptores.
         * ready indica cuántos están preparados.
         */
        for (int fd = 0; fd <= max_fd && ready > 0; fd++) {
            if (!FD_ISSET(fd, &read_set)) {
                continue;
            }

            ready--;

            /*
             * Si el descriptor preparado es server_fd,
             * existe una conexión pendiente.
             */
            if (fd == server_fd) {
                struct sockaddr_in client_address;

                socklen_t client_address_length =
                    sizeof(client_address);

                int client_fd = accept(
                    server_fd,
                    (struct sockaddr *)&client_address,
                    &client_address_length
                );

                if (client_fd < 0) {
                    if (errno == EINTR) {
                        continue;
                    }

                    perror("accept");
                    continue;
                }

                if (client_fd >= FD_SETSIZE) {
                    fprintf(
                        stderr,
                        "Cliente rechazado: descriptor %d "
                        "supera FD_SETSIZE\n",
                        client_fd
                    );

                    close(client_fd);
                    continue;
                }

                char client_ip[INET_ADDRSTRLEN];

                if (inet_ntop(
                        AF_INET,
                        &client_address.sin_addr,
                        client_ip,
                        sizeof(client_ip)) == NULL) {
                    strcpy(
                        client_ip,
                        "desconocida"
                    );
                }

                unsigned int client_port =
                    (unsigned int)ntohs(
                        client_address.sin_port
                    );

                printf(
                    "Cliente conectado desde %s:%u "
                    "con descriptor %d\n",
                    client_ip,
                    client_port,
                    client_fd
                );

                /*
                 * El nuevo socket se añade al conjunto
                 * maestro para vigilarlo en las próximas
                 * llamadas a select().
                 */
                FD_SET(client_fd,  &master_set );

                if (client_fd > max_fd) {
                    max_fd = client_fd;
                }

                continue;
            }

            /*
             * Si no es server_fd, es un cliente con
             * datos disponibles o una conexión cerrada.
             */
            char buffer[BUFFER_SIZE];

            ssize_t received = recv(
                fd,
                buffer,
                sizeof(buffer) - 1,
                0
            );

            if (received < 0) {
                if (errno == EINTR) {
                    continue;
                }

                perror("recv");

                close(fd);
                FD_CLR(fd, &master_set);
                
            } else if (received == 0) {
                printf(
                    "El cliente del descriptor %d "
                    "ha cerrado la conexión\n",
                    fd
                );

                close(fd);
                FD_CLR(fd, &master_set);
            } else {
                buffer[received] = '\0';

                printf(
                    "Mensaje del descriptor %d: %s\n",
                    fd,
                    buffer
                );

                /*
                 * Respuesta tipo eco:
                 * se devuelve al mismo cliente
                 * exactamente lo recibido.
                 */
                if (send_all(
                        fd,
                        buffer,
                        (size_t)received) < 0) {
                    perror("send_all");

                    close(fd);
                    FD_CLR(fd, &master_set);
                }
            }

            /*
             * Si se cerró el descriptor que tenía el
             * valor máximo, se recalcula max_fd.
             */
            if (fd == max_fd &&
                !FD_ISSET(fd, &master_set)) {
                while (max_fd >= 0 &&
                       !FD_ISSET(
                           max_fd,
                           &master_set)) {
                    max_fd--;
                }
            }
        }
    }

    close_all_clients(
        &master_set,
        server_fd,
        max_fd
    );

    close(server_fd);

    return EXIT_FAILURE;
}
