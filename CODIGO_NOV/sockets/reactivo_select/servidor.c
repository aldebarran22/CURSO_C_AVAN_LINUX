#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define PUERTO 8080
#define BACKLOG 4
#define BUFFER_SIZE 1024

static int send_all(int socket_fd, const void *buffer, size_t length){
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

int aceptar_cliente(struct sockaddr_in *client_address, int server_fd){

}


int main(){
  // Crear el socket:
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0){
        perror("socket");
        return EXIT_FAILURE;
    }

    // Reutilizar el puerto:
    int reuse_address = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) < 0){
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    puts("\nSocket creado ....");

    // Configurar la direccion del socket:
    struct sockaddr_in server_address;

    //Inicializar la struct a cero:
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PUERTO);
    // Para que el servidor escuche por todas las interfaces de red:
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Ligar el socket con la configuracion de la direccion:
    if (bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, BACKLOG) < 0){
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("\nSocket escuchando en el puerto: %d, descriptor: %d\n\n", PUERTO, server_fd);

    // Select marca el limite de los descriptores con la macro FD_SETSIZE
    // Todos los descriptores deben ser menores que: FD_SETSIZE
    // Validar el descriptor del socket del servidor:
    if (server_fd >= FD_SETSIZE){
        fprintf(stderr, "El descriptor del servidor es >= FD_SETSIZE: %d\n", server_fd);

        close(server_fd);
        return EXIT_FAILURE;
    }

    // master almacena todos los descriptores que queremos monitorizar:
    fd_set master;

    // Inicializar la la lista:
    FD_ZERO(&master);

    // Añadir el socket del servidor a la lista: master:
    FD_SET(server_fd, &master);

    // El descriptor mas alto  hasta este momento:
    int max_fd = server_fd;

    // Bucle infinito del servidor:
    while (1){

        // Hacemos una copia de master, porque la llamada a seelect modifica
        // el conjunto maestro
        fd_set read_set = master;

        int ready = select(max_fd+1, &read_set, NULL, NULL, NULL);
        if (ready < 0){
            if (errno == EINTR){
                continue;
            }

            perror("select");
            break;
        }

        printf("\nNum. descriptores preparados: %d", ready);

        // Se recorren los posibles descriptores
        // El numero de descriptores preparados es el valor de ready
        for (int fd = 0 ; fd <= max_fd && ready > 0 ; fd++){

            if (!FD_ISSET(fd, &read_set)){
                // Si el descriptor actual no ha producido evento (no esta en la lista)
                // pasamos al siguiente!
                continue;
            }

            ready--;

            // Comprobar si el descriptor es el del servidor:
            if (fd == server_fd){
                struct sockaddr_in client_address;

                // Se va a conectar un cliente y tenemos que aceptarlo!
                int client_fd = aceptar_cliente(&client_address, server_fd);

                // Evaluar client_fd
            }
        }
    }

    // Cerrar las conexiones de los clientes

    close(server_fd);
    return EXIT_FAILURE;
}