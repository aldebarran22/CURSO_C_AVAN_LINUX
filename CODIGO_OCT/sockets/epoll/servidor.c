#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define LISTEN_BACKLOG 10
#define MAX_EVENTS 32
#define BUFFER_SIZE 1024

static int set_nonblocking(int file_descriptor)
{
    // Poner el socket en modo no bloqueante
    // Primero recuperar los flags: F_GETFL
    int flags = fcntl(
        file_descriptor,
        F_GETFL,
        0
    );

    if (flags < 0) {
        return -1;
    }

    // Poner el socket en modo no bloqueante! or a nivel de bits
    // Establecer los flags de estado: F_SETFL
    if (fcntl(
            file_descriptor,
            F_SETFL,
            flags | O_NONBLOCK) < 0) {
        return -1;
    }

    return 0;
}

static int add_to_epoll(
    int epoll_fd,
    int file_descriptor)
{
    struct epoll_event event;

    memset(&event, 0, sizeof(event));

    event.events = EPOLLIN;
    event.data.fd = file_descriptor;

    if (epoll_ctl(
            epoll_fd,
            EPOLL_CTL_ADD,
            file_descriptor,
            &event) < 0) {
        return -1;
    }

    return 0;
}

static void close_client(
    int epoll_fd,
    int client_fd)
{
    if (epoll_ctl(
            epoll_fd,
            EPOLL_CTL_DEL,
            client_fd,
            NULL) < 0) {
        perror("epoll_ctl DEL");
    }

    close(client_fd);

    printf(
        "Cliente desconectado. Descriptor: %d\n",
        client_fd
    );
}

static void accept_clients(
    int server_fd,
    int epoll_fd)
{
    while (1) {
        struct sockaddr_in client_address;

        socklen_t client_address_length =
            sizeof(client_address);

        int client_fd = accept(
            server_fd,
            (struct sockaddr *)&client_address,
            &client_address_length
        );

        if (client_fd < 0) {
            // Se interrupio antes de aceptar la conexion:
            if (errno == EINTR) {
                continue;
            }

            // Se ha  vaciado la cola de conexiones pendientes
            if (errno == EAGAIN ||
                errno == EWOULDBLOCK) {
                break;
            }

            perror("accept");
            break;
        }

        if (set_nonblocking(client_fd) < 0) {
            perror("set_nonblocking cliente");
            close(client_fd);
            continue;
        }

        if (add_to_epoll(
                epoll_fd,
                client_fd) < 0) {
            perror("epoll_ctl ADD cliente");
            close(client_fd);
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];

        if (inet_ntop(
                AF_INET,
                &client_address.sin_addr,
                client_ip,
                sizeof(client_ip)) == NULL) {
            strcpy(client_ip, "desconocida");
        }

        unsigned int client_port =
            (unsigned int)ntohs(
                client_address.sin_port
            );

        printf(
            "Cliente conectado desde %s:%u. "
            "Descriptor: %d\n",
            client_ip,
            client_port,
            client_fd
        );
    }
}

static void attend_client(
    int epoll_fd,
    int client_fd)
{
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (received > 0) {
            buffer[received] = '\0';

            printf(
                "Descriptor %d envía: %s\n",
                client_fd,
                buffer
            );

            const char response[] =
                "Mensaje recibido correctamente\n";

            ssize_t sent = send(
                client_fd,
                response,
                sizeof(response) - 1,
                MSG_NOSIGNAL
            );

            if (sent < 0) {
                if (errno == EINTR) {
                    continue;
                }

                if (errno == EAGAIN ||
                    errno == EWOULDBLOCK) {
                    /*
                     * El búfer de envío está lleno.
                     * Una versión más avanzada registraría
                     * temporalmente EPOLLOUT.
                     */
                    return;
                }

                perror("send");
                close_client(
                    epoll_fd,
                    client_fd
                );

                return;
            }

            continue;
        }

        if (received == 0) {
            close_client(
                epoll_fd,
                client_fd
            );

            return;
        }

        if (errno == EINTR) {
            continue;
        }

        if (errno == EAGAIN ||
            errno == EWOULDBLOCK) {
            /*
             * Se han consumido todos los datos
             * disponibles actualmente.
             */
            return;
        }

        perror("recv");

        close_client(
            epoll_fd,
            client_fd
        );

        return;
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

    // Para que el socket del servidor NO espere  en la operaciones
    // de accept, send y recv. El socket del servidor es solo 1, 
    // y tiene que atender a multiples clientes, no se puede quedar
    // bloqueado con uno concreto.

    // Por ejemplo, si va a leer datos y todavia no hay datos NO
    // se queda esperando, retorna inmediatamente!
    if (set_nonblocking(server_fd) < 0) {
        perror("set_nonblocking servidor");
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
    server_address.sin_port =
        htons(SERVER_PORT);

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

    int epoll_fd = epoll_create1(
        EPOLL_CLOEXEC
    );

    if (epoll_fd < 0) {
        perror("epoll_create1");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (add_to_epoll( epoll_fd, server_fd) < 0) {
        perror("epoll_ctl ADD servidor");
        close(epoll_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf(
        "Servidor epoll escuchando "
        "en el puerto %d\n",
        SERVER_PORT
    );

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int event_count = epoll_wait( epoll_fd, events, MAX_EVENTS,-1);

        if (event_count < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < event_count; i++) {
            int current_fd = events[i].data.fd;

            uint32_t current_events =
                events[i].events;

            if (current_fd == server_fd) {
                accept_clients(
                    server_fd,
                    epoll_fd
                );

                continue;
            }

            if ((current_events & EPOLLERR) != 0 ||
                (current_events & EPOLLHUP) != 0 ||
                (current_events & EPOLLRDHUP) != 0) {
                close_client(
                    epoll_fd,
                    current_fd
                );

                continue;
            }

            if ((current_events & EPOLLIN) != 0) {
                attend_client(
                    epoll_fd,
                    current_fd
                );
            }
        }
    }

    close(epoll_fd);
    close(server_fd);

    return EXIT_SUCCESS;
}