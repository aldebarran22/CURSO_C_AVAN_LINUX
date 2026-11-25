// cliente 1 a 1


#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PUERTO 8080
#define BUFFER_SIZE 1024

int send_all(int client_fd, const void *buffer, size_t length){
    // Para recorrer los bytes:
    const unsigned char* cursor = buffer; // casting implicito
    size_t total = 0;

    while (total < length){

        // Devuelve cuantos bytes realmente se han enviado
        ssize_t enviados = send(client_fd, cursor + total, length-total, 0 );

        if (enviados < 0){
            // Si se ha producido una señal que corte el envio:
            if (errno == EINTR){
                // Reintenta enviar de nuevo
                continue;
            }

            return -1;
        } 

        if (enviados == 0){
            return -1;
        }

        // Hemos conseguido enviar:
        total += (size_t) enviados;
    }
    return 0;
}

int main(){
    int socket_fd = -1;  

    // Crear el socket:
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0){
        perror("socket");
        return EXIT_FAILURE;
    }

    // Configurar la dir. del servidor con el que vamos a conectar:     
    struct sockaddr_in server_address;

    //Inicializar la struct a cero:
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PUERTO);
    
    // Convertir la dir.IP del servidor de texto a binario:
    int resul = inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
    if (resul <= 0){
        perror("int_pton");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (connect(socket_fd, (const struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("\nConectado con el servidor: %s %d", SERVER_IP, PUERTO);
    const char mensaje[] = "mensaje desde el cliente";

    if (send_all(socket_fd, mensaje, sizeof(mensaje)-1) < 0){
        perror("send_all");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    puts("\nSe ha enviado un mensaje al servidor");

    char respuesta[BUFFER_SIZE];
    ssize_t received = recv(socket_fd, respuesta, sizeof(respuesta)-1, 0);
    if (received < 0){
        perror("recv");
        close(socket_fd);
        return EXIT_FAILURE;

    } else if (received == 0){
        puts("\nSe ha cerrado la conexion");
        close(socket_fd);
        return EXIT_FAILURE;

    } 
    respuesta[received] = '\0';
    printf("\nMensaje recibido del servidor: %s", respuesta);        
    
    close(socket_fd);
    return EXIT_SUCCESS;
}