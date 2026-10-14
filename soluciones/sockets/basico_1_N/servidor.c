/* 
Servidor basico de 1 a N:
Se pueden conectar N clientes pero se atienden secuencialmente.
Aceptar cliente 1
Atender cliente 1
Cerrar cliente 1

Aceptar cliente 2
Atender cliente 2
Cerrar cliente 2

Modificar el parametro de listen para que se queden esperando
Indica el numero de conexiones que puede haber esperando, pero
solo hay un cliente activo.
*/


#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define LISTEN_BACKLOG 10


static int send_all(int socket_fd,  const void *buffer, size_t length) {
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        // Entero con signo, por si devuelve -1
        ssize_t sent = send(socket_fd, cursor + total_sent, length - total_sent, 0);

        if (sent < 0) {
            if (errno == EINTR) {
                // EINTR: llamada al sistema interrumpida. 
                // send fue interrupida por una señal
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

static void attend_client(int client_fd, const char* client_ip, unsigned int client_port){
    char buffer[BUFFER_SIZE];

    printf("\nCliente conectado: %s:%u", client_ip, client_port);

    // Recibir datos del cliente: guardar el  nro. de bytes recibidos
    ssize_t received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

    if (received < 0){
        perror("recv");
        return;
    }

    // Marca el final de los bytes:
    buffer[received] = '\0';
    printf("\nDatos recibidos: %s", buffer);

    // Preparar la respuesta al servidor:
    const char response[] = "Mensaje recibido correctamente ...";

    // Se envian los datos al servidor:
    if (send_all(client_fd, response, sizeof(response)-1) < 0){
        perror("send_all");
        return;
    }

    puts("Respuesta enviada al cliente ...");
}


int main(void){
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

    // Numero de clientes en cola LISTEN_BACKLOG
    if (listen(server_fd, LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Servidor escuchando en el puerto %d\n", SERVER_PORT);

    // Bucle infinito para aceptar clientes:
    while (1){
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);

        // Aceptar a un cliente:
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);

        if (client_fd < 0) {

            if (errno == EINTR){
                continue;
            }
            
            perror("accept");
            break;
        }

        char client_ip[INET_ADDRSTRLEN];
        unsigned int client_port = (unsigned int)ntohs(client_address.sin_port);

        // Convierte la dir. IP del cliente de binario a texto:
        if (inet_ntop( AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
            strcpy(client_ip, "desconocida");
        }

        // Atender  al cliente conectado:
        attend_client(client_fd, client_ip, client_port);

        // Cerrar la conexion con el cliente:
        close(client_fd);

        puts("Conexion con el cliente cerrada ...");
        puts("Esperando al siguiente cliente ...");
    }

    // Aunque no se alcanza nunca:  
    close(server_fd);
    return EXIT_SUCCESS;
}