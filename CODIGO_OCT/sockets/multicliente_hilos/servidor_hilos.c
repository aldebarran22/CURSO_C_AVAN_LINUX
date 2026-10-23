// Servidor multicliente: implementacion con hilos

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024
#define LISTEN_BACKLOG 10

typedef struct {
    int client_fd;                      // El socket del cliente
    char client_ip[INET_ADDRSTRLEN];    // la ip del cliente
    unsigned int client_port;           // El puerto del cliente
} client_data_t;


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
    int server_fd = -1;
    

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

    if (listen(server_fd, LISTEN_BACKLOG) < 0){
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("\nServidor escuchando en el puerto: %d\n", PUERTO);

    // El servidor esta funcionando todo el rato:
    while (1){
        puts("\nServidor a la espera de clientes ...");

        // Nos hace falta un socket / dir_cliente por cada cliente que se conecte:        
        struct sockaddr_in client_address;
        socklen_t len_dir_cliente = sizeof(client_address);

        // Aceptar (esperar) a que se conecteun cliente:
        int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &len_dir_cliente);
        if (client_fd < 0){

            if (errno == EINTR){
                continue;
            }

            perror("accept");
            break;
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;

}