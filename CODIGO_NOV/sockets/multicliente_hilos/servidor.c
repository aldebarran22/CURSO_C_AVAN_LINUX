// Servidor 1 a 1

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024
#define BACKLOG 4

typedef struct {
    int client_fd;                      // El socket del cliente
    char client_ip[INET_ADDRSTRLEN];    // la ip del cliente
    unsigned int client_port;           // El puerto del cliente
} client_data_t;


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


void *client_handler(void *p){
    /*
        char buffer[BUFFER_SIZE];

        ssize_t received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

        if (received < 0){
            // Se ha producido un error:
            perror("recv");
            close(server_fd);
            close(client_fd);
            return EXIT_FAILURE;

        } else if (received == 0){
            // El cliente ha cerrado la conexion sin enviar datos
            puts("El cliente cerro la conexion");

        } else {
            // Se han recibido datos:
            buffer[received]='\0';
            printf("\nSe ha recibido el mensaje: %s", buffer);

            const char mensaje[] = "Mensaje recibido";
            if (send_all(client_fd, mensaje, sizeof(mensaje)-1) < 0){
                perror("sendall");
                close(client_fd);
                close(server_fd);
                return EXIT_FAILURE;
            }
        }*/

    return NULL;
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

    if (listen(server_fd, BACKLOG) < 0){
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("\nSocket escuchando en el puerto: %d", PUERTO);

    // Bucle principal del servidor para atender multiples clientes:
    while (1){
        puts("\nServidor a la espera de clientes");

        // Direccion del cliente:
        struct sockaddr_in client_address;
        socklen_t len_dir_cliente = sizeof(client_address);

        // Aceptar a un nuevo cliente:
        client_fd = accept(server_fd, (struct sockaddr *) &client_address, &len_dir_cliente);        
        if (client_fd < 1){

            if (errno == EINTR){
                continue;
            }

            perror("accept");
            break;            
        }

        // Reservar memoria para el cliente conectado:
        client_data_t *client_data = malloc(sizeof(client_data_t));
        if (client_data == NULL){
            perror("malloc");
            close(client_fd);
            continue;
        }

        // Rellenar la estructura del cliente:
        client_data->client_fd = client_fd; // Copiar el socket

        // Convertir la dir IP del cliente de binario a texto:        
        if (inet_ntop(AF_INET, &client_address.sin_addr, client_data->client_ip, sizeof(client_data->client_ip)) == NULL){
            strcpy(client_data->client_ip, "IP desconocida");
        }
        client_data->client_port = (unsigned int) ntohs(client_address.sin_port);

        // El puerto viene en formato de la red y se convierte al host:
        // de big endian a little endian
        printf("\nCliente conectado: %s %u", client_data->client_ip, client_data->client_port);

        // Crear el hilo:
        pthread_t hilo;
        int error = pthread_create(&hilo, 0, client_handler, client_data);
        if (error){
            printf("\nError al lanzar el hilo del cliente");
            close(client_fd);
            free(client_data);
            client_data = NULL;
            continue;
        }

        // Desconectar el hilo:
        error = pthread_detach(hilo);
        if (error){
            puts("\nError al desconectar el hilo del cliente");
        }        
    }
    
    close(server_fd);
    return EXIT_SUCCESS;
}