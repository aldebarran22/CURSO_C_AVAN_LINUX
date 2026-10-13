// Servidor  basico

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <error.h>
#include <arpa/inet.h>

#define PUERTO 3490 // Puerto para conectar
#define BACKLOG 10  // Numero de conexiones pendientes ,en cola



int main(){
    int sock, sock_client;
    struct sockaddr_in dir_Servidor;
    struct sockaddr_in dir_Cliente;
    int sin_size;
    const char* mensaje = "Hello World\n";

    // Crear el socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("Error al crear el socket");
        exit(1);
    }
    puts("\nSe ha creado el socket correctamente ...");

    // Configurar la direccion del servidor:
    dir_Servidor.sin_family = AF_INET;      // Ordenacion de bytes de la maquina
    dir_Servidor.sin_port = htons(PUERTO);  // Ordenacion de bytes  de la red
    dir_Servidor.sin_addr.s_addr = INADDR_ANY; // Rellenar con la IP
    memset(&(dir_Servidor.sin_zero), '\0', 8); // Rellenar con ceros el resto de la estructura


    // Hacer el bind:
    if (bind(sock, (struct sockaddr *)&dir_Servidor, sizeof(struct sockaddr))==-1){
        perror("Error al hacer bind");
        exit(1);
    }
    puts("\nTodo correcto al hacer bind ...");

    // Configurar listen:
    if (listen(sock, BACKLOG)==-1){
        perror("Error al configurar listen");
        exit(1);
    }
    puts("\nTodo correcto al configurar listen");

    // Bucle infinito para aceptar conexiones de los clientes: accept()
    // Sin hilos ni fork, podemos aceptar varios clientes pero siempre uno detras de otro.
    while (1){
        // Almacenar el tamaño de la direccion del cliente
        sin_size = sizeof(struct sockaddr_in);

        puts("\nServidor a la espera de clientes ...");
        
        if ((sock_client = accept(sock_client, (struct sockaddr *) &dir_Cliente, &sin_size))== -1){
            perror("Error al acceptar una conexion de un cliente");
            exit(1);
        }

        // El cliente ha sido aceptado:
        printf("Servidor: conexion establecida con el cliente: %s\n", inet_ntoa(dir_Cliente.sin_addr));

        // Se crea un proceso hijo: devuelve 0 al proceso hijo, y el PID al padre
        if (!fork()){
            
            // El  proceso hijo  cierra el socket del servidor
            close(sock);

            // Envia datos al servidor:
            if (send(sock_client, mensaje, strlen(mensaje),0)== -1){
                perror("Error al enviar un mensaje desde el cliente");
                close(sock_client);
                exit(2); 
            }


        }

        // El padre no necesita el socket:
        close(sock_client);
    }
    return 0;

}