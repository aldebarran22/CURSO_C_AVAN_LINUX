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


void atender_client(int client_fd, const char *ip, unsigned int port){
    char buffer[BUFFER_SIZE];

    printf("\nHilo %lu atendiendo a %s:%u\n", (unsigned long)pthread_self(), ip, port);

    ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (received < 0) {
        perror("recv");
        return;
    }

    if (received == 0) {
        puts("El cliente cerró la conexión sin enviar datos.");
        return;
    }

    buffer[received] = '\0';
    printf("Datos recibidos de %s:%u: %s\n", ip, port, buffer);
    const char response[] = "Mensaje recibido correctamente ...";

    if (send_all(client_fd, response, sizeof(response) - 1) < 0) {
        perror("send_all");
        return;
    }

    puts("Respuesta enviada al cliente.");
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

       
        char client_ip[INET_ADDRSTRLEN];

        // Convertir la dir IP del cliente de binario a texto:        
        if (inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL){
            strcpy(client_ip, "IP desconocida");
        }
        unsigned int client_port = (unsigned int) ntohs(client_address.sin_port);

        // El puerto viene en formato de la red y se convierte al host:
        // de big endian a little endian
        printf("\nCliente conectado: %s %u", client_ip, client_port);

        // Crear el proceso hijo:
        // El proceso padre recibe el pid del proceso hijo
        // El proceso hijo recibe un 0
        puts("\nCrear el proceso hijo para atender al cliente");
        pid_t pid = fork();
        
        if (pid == 0){
            // Es el proceso hijo, es el que atiende al cliente:
            // El proceso no necesita el socket del servidor:
            close(server_fd);

            // El proceso hijo atiende al cliente:
            atender_client(client_fd, client_ip, client_port);
            close(client_fd);

            // Terminar el proceso hijo:
            _exit(EXIT_SUCCESS);


        } else {
            printf("\nSe ha creado un proceso hijo con el PID: %ld", pid);

            // El proceso padre no necesita el socket del hijo, lo cierra:
            close(client_fd);
        }
    }
    
    close(server_fd);
    return EXIT_SUCCESS;
}