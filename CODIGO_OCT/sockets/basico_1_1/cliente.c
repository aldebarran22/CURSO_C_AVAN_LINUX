
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


