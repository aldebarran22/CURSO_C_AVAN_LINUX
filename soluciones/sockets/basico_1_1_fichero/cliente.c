// Cliente  para conectar con un servidor
// La conexion es 1 a 1

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define PATH_FILE "pedidos_final.csv"


static int send_all(int socket_fd, const void *buffer, size_t length){
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
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


static int send_file(const char* path_file, int socket_fd){

    // Validar el path del fichero:
    if (path_file == NULL){
        fprintf(stderr, "\nLa ruta del fichero es NULL\n");
        return -1;
    }

    // Validar la longitud del path:
    size_t path_length = strlen(path_file);

    if (path_length == 0 || path_length > BUFFER_SIZE){
        fprintf(stderr, "\nLa longitud del path del fichero no es correcta: %zu\n", path_length);
        return -1;
    }

    // Primero: se transmite un entero que contiene el tamaño fijo y en orden de bytes de la red
    uint32_t red_path_length = htonl((uint32_t) path_length);

    // Enviar la longitud del path del fichero:
    if (send_all(socket_fd, &red_path_length, sizeof(red_path_length)) < 0){
        perror("send longitud del nombre del fichero");
        return EXIT_FAILURE;
    } 
    printf("\nSe envia la longitud del nombre del fichero: %zu chars\n", path_length);


    // Segundo: se transmite el path del fichero:
    if (send_all(socket_fd,  path_file, path_length) < 0){
        perror("send nombre del fichero");
        return EXIT_FAILURE;
    }

    printf("\nSe ha enviado el nombre del fichero: %s\n", path_file);


    // Tercero: hay que enviar el tamaño del fichero:
    FILE *f;
    char path_dir_file[BUFFER_SIZE] = "ficheros_cliente/";
    strcat(path_dir_file, path_file);

    // El fichero se abre en modo binario;
    if ((f = fopen(path_dir_file, "rb"))== NULL){
        perror("No se encuentra el path  del fichero a enviar");
        return EXIT_FAILURE;
    }

    // Definir una estructura stat para recuperar el tamaño del fichero:
    struct stat informacion_fichero;

    if (stat(path_dir_file, &informacion_fichero) < 0){
        perror("stat");
        return EXIT_FAILURE;
    }

    if (!S_ISREG(informacion_fichero.st_mode)){
        fprintf(stderr, "\nLa ruta no pertenece a un fichero regular ...\n");
        return EXIT_FAILURE;
    }


    if (informacion_fichero.st_size < 0){
        fprintf(stderr, "\nEl size del fichero no es valido ...\n");
        return EXIT_FAILURE;
    }

    // Preparamos en formato de red el tamaño en bytes del fichero a transmitir:
    uint64_t file_size = informacion_fichero.st_size;
    uint64_t red_file_size = htonl((uint64_t) file_size);


    // Enviar el size del  fichero:
    if (send_all(socket_fd, &red_file_size, sizeof(red_file_size)) < 0){
        perror("send size del fichero");
        return EXIT_FAILURE;
    } 
    printf("\nSe envia el size del fichero: %zu bytes\n", file_size);




    // Cuarto: hay que enviar el contenido del fichero:


    fclose(f);
    return 0;

    
    /*
    const char message[] = "Hola desde el cliente";

    if (send_all(socket_fd, message, sizeof(message) - 1) < 0) {
        perror("send");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("Mensaje enviado: %s\n", message);
    char buffer[BUFFER_SIZE];
    ssize_t received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);

    if (received < 0) {
        perror("recv");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (received == 0) {
        puts("El servidor cerró la conexión sin enviar una respuesta.");
        close(socket_fd);
        return EXIT_SUCCESS;
    }

    buffer[received] = '\0';

    printf("Respuesta del servidor: %s\n", buffer);*/
}


int main(void){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    int conversion_result = inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (conversion_result == 0) {
        fprintf(stderr, "Dirección IP no válida: %s\n", SERVER_IP);
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (conversion_result < 0) {
        perror("inet_pton");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (connect(socket_fd, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("Conectado al servidor %s:%d\n", SERVER_IP, SERVER_PORT);

    if (send_file(PATH_FILE, socket_fd) < 0){
        fprintf(stderr, "\nNo se ha podido enviar el fichero %s", PATH_FILE);
        close(socket_fd);
        return EXIT_FAILURE;
    }
  
    close(socket_fd);
    return EXIT_SUCCESS;
}