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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <error.h>
#include <errno.h>
#include <arpa/inet.h>


#define PUERTO          3490    // Puerto para conectar
#define BUFFER_SIZE     1024    // Size del buffer.
#define LISTEN_BACKLOG  10      // Numero de conexiones pendientes ,en cola


static void collect_children(int signal_number){
    // Esta funcion recoge procesos hijos que ya han terminado, para que no se queden
    // como procesos zombies!

    // Es la funcion manejadora de SIGCHLD
    // Cuando uno de los procesos hijo termina, el proceso padre recibe SIGCHLD y se ejecuta
    // collect_children(SIGCHLD);

    // Guardar el numero de error:
    int saved_errno = errno;

    // Evitar warnning si no se utiliza el parametro:
    (void) signal_number;

    // waitpid puede modificar errno
    // -1, cualquier proceso hijo terminado
    // El param 2, permite recoger el estado de finalizacion del proceso hijo: int status
    // WNOHANG para que waitpid no  se bloquee si no hay ningun proceso hijo terminando!
    while (waitpid(-1, NULL, WNOHANG) > 0){}

    // Si devuelve 0, hay procesos hijo pero no han terminado

    // Volver a restaurar el codigo de error!
    errno = saved_errno;
}



static  int configure_sigchld(void){
    struct sigaction action;

    // Inicializar a cero la estructura:
    memset(&action, 0, sizeof(action));

    // Configurar el action:
    action.sa_handler = collect_children;
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigemptyset(&action.sa_mask) < 0){
        return -1;
    }


    if (sigaction(SIGCHLD, &action, NULL) < 0){
        return -1;
    }

    return 0;
}


static int send_all(int socket_fd,  const void *buffer, size_t length) {
    const unsigned char *cursor = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        // Entero con signo, por si devuelve -1
        ssize_t sent = send(socket_fd, cursor + total_sent, length - total_sent, 0);

        if (sent < 0) {
            if (errno == EINTR) {
                // EINTR: llamada al sistema interrumpida. 
                // send fue interrupida por una señal
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


static void attend_client(int client_fd, const char* client_ip, unsigned int client_port){
    char buffer[BUFFER_SIZE];

    printf("\nProceso hijo %ld atendiendo a %s:%u", (long)getgid(), client_ip, client_port);

    // Recibir datos del cliente: guardar el  nro. de bytes recibidos
    ssize_t received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

    if (received < 0){
        perror("recv");
        return;
    }

    if (received == 0){
        puts("El cliente cerro la conexion, sin enviar datos ...");
        return;
    }

    // Marca el final de los bytes:
    buffer[received] = '\0';
    printf("\nDatos recibidos de %s:%u: %s", client_ip, client_port, buffer);

    // Preparar la respuesta al servidor:
    const char response[] = "Mensaje recibido correctamente ...";

    // Se envian los datos al servidor:
    if (send_all(client_fd, response, sizeof(response)-1) < 0){
        perror("send_all");
        return;
    }

    puts("Respuesta enviada al cliente ...");
}




int main(){
    int sock, sock_client;
    struct sockaddr_in dir_Servidor;
    struct sockaddr_in dir_Cliente;
    int sin_size;
    const char* mensaje = "Hello World\n";

    // Evitar problemas con buffers duplicados despues de fork().
    // La salida se escribe inmediatamente
    setvbuf(stdout, NULL, _IONBF, 0);

    if (configure_sigchld() < 0){
        perror("configure_sigchld");
        return EXIT_FAILURE;
    }

    // Crear el socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("Error al crear el socket");
        exit(1);
    }    
  

    // Reutilizar la direccion:
    int reuse_address = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) < 0) {
        perror("setsockopt");
        close(sock);
        return EXIT_FAILURE;
    }

    memset(&dir_Servidor, 0, sizeof(dir_Servidor));

    // Configurar la direccion del servidor:
    dir_Servidor.sin_family = AF_INET;      // Ordenacion de bytes de la maquina
    dir_Servidor.sin_port = htons(PUERTO);  // Ordenacion de bytes  de la red
    dir_Servidor.sin_addr.s_addr = INADDR_ANY; // Rellenar con la IP
    memset(&(dir_Servidor.sin_zero), '\0', 8); // Rellenar con ceros el resto de la estructura


    // Hacer el bind:
    if (bind(sock, (struct sockaddr *)&dir_Servidor, sizeof(struct sockaddr))==-1){
        perror("Error al hacer bind");
        close(sock);
        exit(1);
    }
   

    // Configurar listen:
    if (listen(sock, LISTEN_BACKLOG)==-1){
        perror("Error al configurar listen");
        exit(1);
    }
   

    printf("\nPID del proceso servidor: %ld\n", (long) getpid());


    // Bucle infinito para aceptar conexiones de los clientes: accept()
    // Sin hilos ni fork, podemos aceptar varios clientes pero siempre uno detras de otro.
    while (1){
        // Almacenar el tamaño de la direccion del cliente
        sin_size = sizeof(struct sockaddr_in);

        puts("\nServidor a la espera de clientes ...");
        
        if ((sock_client = accept(sock_client, (struct sockaddr *) &dir_Cliente, &sin_size))== -1){
            
            if (errno == EINTR){
                continue;
            }
            
            perror("Error al acceptar una conexion de un cliente");
            exit(1);
        }


        // Capturar la direccion  IP y el puerto del cliente:
        char client_ip[INET_ADDRSTRLEN];

        // dir IP del cliente:
        if (inet_ntop(AF_INET, &dir_Cliente.sin_addr, client_ip, sizeof(client_ip))==NULL){
            strcpy(client_ip, "desconocida");
        }

        // port del cliente
        unsigned int client_port = (unsigned int) ntohs(dir_Cliente.sin_port);


        // El cliente ha sido aceptado:
        printf("Nueva conexion establecida desde %s:%u\n", client_ip, client_port);


        // Se crea un proceso hijo para atender al cliente: 
        // Devuelve 0 al proceso hijo, y el PID al padre
        pid_t child_pid = fork();


        if (child_pid == 0){
            
            // El  proceso hijo  cierra el socket del servidor, no acepta mas conexiones!
            close(sock);

            attend_client(sock_client, client_ip, client_port);

            close(sock_client);

            // Termina el proceso hijo:
            exit(EXIT_SUCCESS);
        }

        // El proceso padre:
        printf("\nCreado el proceso hijo con PID %ld\n", (long) child_pid);
        

        // El padre no necesita el socket del cliente conectado:
        close(sock_client);

        // Vuelve de nuevo al accept para aceptar nuevas conexiones!
    }

    close(sock);
    return EXIT_SUCCESS;

}