// Servidor basico: 1 a 1

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024



int main(){
    int server_fd = -1;
    int client_fd = -1;

    // Crear el socket:
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0){
        perror("socket");
        return EXIT_FAILURE;
    }

    // Reutilizar la direccion:
    int  reuse_address = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address))< 0){
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Configurar la direccion del socket:
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PUERTO);
    // Para que el servidor escuche de todas las interfaces de red
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Ligar el socket a la configuracion de la direccion:
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

    printf("\nServidor escuchando en el puerto: %d\n", PUERTO);

    // La direccion del cliente:
    struct sockaddr_in client_address;
    socklen_t len_dir_cliente = sizeof(client_address);

    // Aceptar (esperar) a que se conecteun cliente:
    client_fd = accept(server_fd, (struct sockaddr *) &client_address, &len_dir_cliente);

    if (client_fd < 0){
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    //  El cliente se ha conectado, y queremos imprimir la direccion del cliente
    // se convierte de binario a texto:
    char client_ip[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL){
        strcpy(client_ip, "desconocida");
    }

    printf("\nCliente conectado: %s:%u", client_ip, (unsigned int) ntohs(client_address.sin_port));
}