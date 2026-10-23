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
}