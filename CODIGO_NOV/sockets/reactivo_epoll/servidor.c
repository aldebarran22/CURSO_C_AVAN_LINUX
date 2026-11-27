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


static int add_to_epoll(int epoll_fd, int file_descriptor){
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

int main(void)
{
    int server_fd = socket(AF_INET,SOCK_STREAM,  0);

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
    /*
    if (set_nonblocking(server_fd) < 0) {
        perror("set_nonblocking servidor");
        close(server_fd);
        return EXIT_FAILURE;
    }*/

    struct sockaddr_in server_address;

    memset(&server_address,   0,   sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

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

    // Crear la lista de descriptores con epoll:
    int epoll_fd = epoll_create1(EPOLL_CLOEXEC);

    if (epoll_fd < 0){
        perror("epoll_create1");
        close(server_fd);
        return EXIT_FAILURE;
    }


}