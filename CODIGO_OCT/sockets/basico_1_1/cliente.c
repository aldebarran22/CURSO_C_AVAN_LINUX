
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"


int send_all(int fd_client, const void *buffer, size_t length){
    // Convertir el buffer a unsigned char
    const unsigned char *cursor = buffer; // casting implicito
    size_t total = 0; // control del numero de bytes enviados

    // metemos el bucle porque nadie asegura que se vaya a enviar todos los datos
    // con un unico send, 
    while (total < length){

        // ojo con sign, por si devuelve -1 la llamada a send
        // cursor + total, se intentan enviar los bytes que nos quedan
        // la primera vez son todos: length
        ssize_t enviados = send(fd_client, cursor + total, length - total, 0);

        if (enviados < 0){
            // por si se produce una señal que interrupe en el envio
            if (errno == EINTR){
                continue;
            }

            return -1; 
        }

        if (enviados == 0){
            return -1;
        }

        // Ya sabemos que hemos enviado datos:
        total += (size_t) enviados;

    }

    return 0;
}


int main(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0){
        perror("socket");
        return EXIT_FAILURE;
    }

    // Configurar la dir del servidor con el que vamos a conectar:
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PUERTO);

    // Convertir la IP de texto a binario:
    int result = inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (result == 0){
        printf("\nDireccion IP no valida: %s", SERVER_IP);
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if  (result < 0){
        perror("inet_pton");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    // Conectar con el servidor:
    if (connect(socket_fd, (const struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("\nConectado al servidor: %s:%d", SERVER_IP, PUERTO);
    char mensaje[] = "mensaje desde el cliente";

    if (send_all(socket_fd, mensaje, sizeof(mensaje)-1) < 0){
        perror("send_all");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    puts("\nSe ha enviado el mensaje al servidor");

    // Recibir la respuesta:
    char buffer[BUFFER_SIZE];
    ssize_t received = recv(socket_fd, buffer, sizeof(buffer)-1, 0);
    

}
