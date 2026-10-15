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

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024



static int send_all(int socket_fd,  const void *buffer, size_t length) {
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        // Entero con signo, por si devuelve -1
        ssize_t sent = send(socket_fd, cursor + total_sent, length - total_sent, 0);

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


static int receive_file_name(int socket_fd, char* file_name, int file_name_size){

    uint32_t network_name_length;

    if (file_name == NULL || file_name_size == 0){
        fprintf(stderr, "Nombre del fichero o size vacios ...");
        return -1;
    }

    // Primero recibimos el size del nombre del fichero:
    int resul = recv_all(socket_fd, &network_name_length, sizeof(network_name_length));
    if (resul < 0){
        perror("recv longitud del nombre");
        return -1;
    }

    if (resul == 0){
        fprintf(stderr, "\nProbablemente el cliente cerro la conexion antes de enviar el size del nombre\n");
        return -1;
    }

    // Se ha recibido la longitud del nombre y hay que ordenarlo segun los bytes de la maquina
    uint32_t name_length = ntohl((uint32_t) network_name_length);


    if (name_length == 0){
        fprintf(stderr, "Se ha recibido una longitud vacia");
        return -1;
    }

    // Si la longitud esta bien, tenemos que recibir el nombre del fichero:
   

    resul = recv_all(socket_fd, file_name, (size_t) name_length);
    if (resul < 0){
        perror("recv nombre del fichero");
        return -1;
    }

    if (resul == 0){
        fprintf(stderr, "\nProbablemente el cliente cerro la conexion antes de enviar el path del fichero\n");
        return -1;
    } 

    // Tenemos el nombre del fichero:
    file_name[name_length] = '\0';

    return 0;
}


int receive_file(int socket_fd){
    char file_name[BUFFER_SIZE];    
    uint64_t network_file_size;

    if (receive_file_name(socket_fd, file_name, sizeof(file_name)) < 0){
        return -1;
    }

    printf("\nNombre del fichero recibido: %s\n", file_name);


    // Ya tenemos el nombre del fichero, ahora se puede recibir el tamaño del fichero:    
    int resul = recv_all(socket_fd, &network_file_size, sizeof(network_file_size));
    if (resul < 0){
        perror("recv longitud del fichero");
        return -1;
    }

    if (resul == 0){
        fprintf(stderr, "\nProbablemente el cliente cerro la conexion antes de enviar el size del nombre\n");
        return -1;
    }

    // Se ha recibido el tamaño del nombre y hay que ordenarlo segun los bytes de la maquina
    uint64_t file_size = ntohl((uint64_t) network_file_size);


    if (file_size == 0){
        fprintf(stderr, "Se ha recibido un size a cero");
        return -1;
    }

    printf("\nEl numero de bytes a recibir del fichero es: %zu\n", file_size);

    // Ahora hay que abrir en modo escritura el fichero:
    FILE *f;
    char path_dir_file[BUFFER_SIZE] = "ficheros_servidor/";
    strcat(path_dir_file, file_name);

    if ((f = fopen(path_dir_file, "wb"))== NULL){
        perror("Error al crear el fichero en el servidor");
        return EXIT_FAILURE;
    }

    
    fclose(f);
    return 0;
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

    // Cola de conexiones pendientes, solo se ejecuta una vez: accept
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Servidor escuchando en el puerto %d\n", SERVER_PORT);

    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    char client_ip[INET_ADDRSTRLEN];

    // Convierte la dir. IP del cliente de binario a texto:
    if (inet_ntop( AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
        strcpy(client_ip, "desconocida");
    }

    printf("Cliente conectado desde %s:%u\n", client_ip, (unsigned int)ntohs(client_address.sin_port));

    // El cliente esta conectado, podemos empezar a recibir los datos del fichero:

    if (receive_file(client_fd) < 0){
        fprintf(stderr, "No se pudo recibir el fichero\n");

    } else {
        puts("\nFichero recibido correctamente ...");
    }
    

    close(client_fd);
    close(server_fd);

    return EXIT_SUCCESS;
}