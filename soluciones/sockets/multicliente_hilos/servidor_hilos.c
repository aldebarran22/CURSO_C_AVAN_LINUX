/* 
Servidor que utiliza hilos para atender a los clientes
Por cada cliente que se conecta se crea un  hilo

*/

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define LISTEN_BACKLOG 10

typedef struct {
    int client_fd;
    char client_ip[INET_ADDRSTRLEN];
    unsigned int client_port;
} client_data_t;


static int send_all(int socket_fd, const void *buffer, size_t length){
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        ssize_t sent = send(socket_fd, cursor + total_sent, length - total_sent, MSG_NOSIGNAL);

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



static void attend_client(int client_fd, const char *client_ip,  unsigned int client_port){
    char buffer[BUFFER_SIZE];

    printf("\nHilo %lu atendiendo a %s:%u\n", (unsigned long)pthread_self(), client_ip, client_port);

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

    printf("Datos recibidos de %s:%u: %s\n", client_ip, client_port, buffer);

    const char response[] = "Mensaje recibido correctamente ...";

    if (send_all(client_fd, response, sizeof(response) - 1) < 0) {
        perror("send_all");
        return;
    }

    puts("Respuesta enviada al cliente.");
}


static void *client_thread(void *argument){
    client_data_t *client_data = argument;

    /*
     * Copiamos los datos que necesitaremos antes
     * de liberar la estructura dinámica.
     */
    int client_fd = client_data->client_fd;
    char client_ip[INET_ADDRSTRLEN];
    memcpy(client_ip, client_data->client_ip, sizeof(client_ip));

    unsigned int client_port = client_data->client_port;

    /*
     * La estructura fue reservada por el hilo
     * principal específicamente para este hilo.
     */
    free(client_data);

    attend_client( client_fd, client_ip, client_port);

    /*
     * Cada hilo cierra exclusivamente el socket del cliente que está atendiendo.
     */
    close(client_fd);
    printf("Hilo %lu finalizado.\n", (unsigned long)pthread_self());
    return NULL;
}

int main(void){

    setvbuf(stdout, NULL, _IONBF, 0);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int reuse_address = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

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

    if (listen( server_fd, LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Servidor con hilos escuchando en el puerto %d\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_address;

        socklen_t client_address_length = sizeof(client_address);

        puts("\nServidor a la espera de clientes ...");

        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);

        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("accept");
            break;
        }

        /*
         * Se reserva una estructura independiente
         * para cada hilo.
         */
        client_data_t *client_data = malloc(sizeof(*client_data));

        if (client_data == NULL) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        client_data->client_fd = client_fd;

        if (inet_ntop(AF_INET, &client_address.sin_addr, client_data->client_ip, sizeof(client_data->client_ip))  == NULL) {
            strcpy(client_data->client_ip, "desconocida" );
        }

        client_data->client_port = (unsigned int)ntohs(client_address.sin_port);

        printf("Nueva conexión desde %s:%u\n", client_data->client_ip, client_data->client_port);

        pthread_t thread;

        int error = pthread_create(&thread, NULL, client_thread,  client_data );

        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error) );

            close(client_fd);
            free(client_data);
            continue;
        }

        /*
         * No se realizará pthread_join().
         * Los recursos internos del hilo se
         * liberarán automáticamente al terminar.
         */
        error = pthread_detach(thread);

        if (error != 0) {
            fprintf(stderr, "pthread_detach: %s\n", strerror(error));
        }

        /*
         * El hilo principal NO cierra client_fd.
         * El descriptor pertenece al mismo proceso
         * y lo cerrará client_thread().
         *
         * El hilo principal vuelve a accept().
         */
    }

    close(server_fd);

    return EXIT_SUCCESS;
}