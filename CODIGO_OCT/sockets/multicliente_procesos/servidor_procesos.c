// Servidor multicliente: implementacion con procesos

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


/* 
Servidor que utilizaprocesos para atender a los clientes
Por cada cliente que se conecta se lanza un proceso con fork()

Proceso padre
    accept()

    fork()

        Padre: cierra el client_fd y vuelve a accept()

        Hijo: cierra server_fd, atiende al cliente,
              cierra client_fd y finaliza


El cliente no cambia.

El padre:
- acepta conexiones;
- crea un hijo;
- cierra su copia de client_fd;
- vuelve inmediatamente a accept().

El hijo:
- cierra su copia de server_fd;
- atiende al cliente;
- cierra client_fd;
- termina mediante _exit().

Además:
- hay que recoger los hijos terminados;
- hay que evitar zombis con waitpid();
- los datos del padre y del hijo no quedan compartidos;
- hay que controlar los errores de fork().              

*/