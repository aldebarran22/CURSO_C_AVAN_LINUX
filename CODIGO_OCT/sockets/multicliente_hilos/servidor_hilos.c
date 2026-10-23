// Servidor multicliente: implementacion con hilos

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

void *client_thread(void *p){
    client_data_t *data = p;

    // Extraer los datos de la estructura dinamica y la liberamos
    // Copiamos los datos a 3 variables y liberamos:
    int client_fd = data->client_fd;
    char ip[INET_ADDRSTRLEN];
    memcpy(ip, data->client_ip, sizeof(ip));

    unsigned int port = data->client_port;

    free(data);
    data = NULL;

    atender_client(client_fd, ip, port);
    close(client_fd);

    printf("\nFinaliza el hilo: %ld con ip:%s:%u", pthread_self(), ip, port);
    return NULL;
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

        // Reservar para la estructura del cliente:
        client_data_t *client_data = malloc(sizeof(*client_data));
        if (client_data == NULL){
            perror("malloc");
            close(client_fd);
            continue;
        }

        // Rellenar la estructura:
        client_data->client_fd = client_fd;

        // Recuperar la IP y el puerto del cliente.
        if (inet_ntop(AF_INET, &client_address.sin_addr, client_data->client_ip, sizeof(client_data->client_ip)) == NULL){
            strcpy(client_data->client_ip, "desconocida");
        }
        client_data->client_port = (unsigned int) ntohs(client_address.sin_port);

        printf("\nNuevo cliente conectado: %s:%u", client_data->client_ip, client_data->client_port);
       
        // Crear un hilo
        pthread_t thread;
        int error = pthread_create(&thread, NULL, client_thread, client_data);
        if (error){
            printf("\nError al crear el hilo: %s", strerror(error));
            close(client_fd);
            free(client_data);
            client_data = NULL;
            continue;
        }

        error = pthread_detach(thread);
        if (error) {
            printf("\nError al detach el hilo: %s", strerror(error));
        }
       
    }

    close(server_fd);
    return EXIT_SUCCESS;

}