// Servidor basico de 1 a 1:
// Solo se conecta un cliente
// En este caso el cliente va a enviar el nombre y contenido 
// de un fichero.

/*
Cliente envía                       Servidor recibe

1. Longitud del nombre      ->      1. Longitud del nombre
2. Nombre del fichero       ->      2. Nombre del fichero
3. Tamaño del fichero       ->      3. Tamaño del fichero
4. Contenido del fichero    ->      4. Contenido del fichero
*/

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024


static int recv_all(int socket_fd, void* buffer, size_t length){
    unsigned  char *cursor = buffer;
    size_t total_received = 0;

    while (total_received < length){
        ssize_t received = recv(socket_fd, cursor + total_received, length - total_received, 0);

        if (received < 0){

            if (errno == EINTR){
                continue;
            }

            return -1;
        }

        if  (received == 0){
            return 0;
        }

        total_received += (size_t) received;
    }

    return 1;

}

int main(){
    int server_fd = -1;
    int client_fd = -1;

    // Crear el socket:
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

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

    if (listen(server_fd, 1) < 0){
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("\nSocket escuchando en el puerto: %d", PUERTO);

    // Direccion del cliente:
    struct sockaddr_in client_address;
    socklen_t len_dir_cliente = sizeof(client_address);

    client_fd = accept(server_fd, (struct sockaddr *) &client_address, &len_dir_cliente);
    if (client_fd < 1){
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Convertir la dir IP del cliente de binario a texto:
    char client_ip[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL){
        strcpy(client_ip, "IP desconocida");
    }

    // El puerto viene en formato de la red y se convierte al host:
    // de big endian a little endian
    printf("\nCliente conectado: %s %u", client_ip, (unsigned int) ntohs(client_address.sin_port));

    if (received_file(client_fd) < 0){
        perror("received_file");
        close(server_fd);
        close(client_fd);
        return EXIT_FAILURE;

    } else {
        puts("Fichero recibido correctamente ...");
    }

    close(server_fd);
    close(client_fd);

    return EXIT_SUCCESS;
}

