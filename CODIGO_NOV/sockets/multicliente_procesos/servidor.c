// Servidor 1 a 1

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>


#define PUERTO 8080
#define BUFFER_SIZE 1024
#define BACKLOG 4


/* 
Servidor que utiliza procesos para atender a los clientes
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
- crea un proceso hijo;
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



static  int configure_sigchild(void){
    // Asocia la funcion collect_children con la señal: SIGCHLD
    struct sigaction action;

    // Inicializar a cero la estructura:
    memset(&action, 0, sizeof(action));

    // Configurar el action:
    action.sa_handler = collect_children;

    // SA_RESTART: ace reiniciables determinadas llamadas al sistema cuando son interrumpidas por señales, 
    // aunque no garantiza el reinicio de absolutamente todas las llamadas
    // SA_NOCLDSTOP: el proceso padre recibe la notificacion que interesa: el fin del proceso hijo
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    // action.sa_mask: conjunto de señales que se bloquearan mientras se ejecuta la funcion
    // del manejador!

    // sigemptyset: Hace reiniciables determinadas llamadas al sistema cuando son interrumpidas 
    // por señales, aunque no garantiza el reinicio de absolutamente todas las llamadas
    if (sigemptyset(&action.sa_mask) < 0){
        return -1;
    }

    // SIGCHLD la señal que queremos configurar
    // action la configuracionde la señal
    // NULL indica que no queremos recuperar la configuracion anterior
    // si valor devuelto es 0, la configuracion es correcta!
    if (sigaction(SIGCHLD, &action, NULL) < 0){
        return -1;
    }

    return 0;
}

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

    printf("\nProceso con PID: %d atendiendo a %s:%u\n", getpid(), ip, port);

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

    // Para evitar problemas de buffers duplicados despues de llamar a fork()
    // La salida se escribe inmediatamente:
    setvbuf(stdout, NULL, _IONBF, 0);

    // Configurar una señal para recoger los procesos zombie!
    if (configure_sigchild() < 0){
        perror("configure_sigchild");
        return EXIT_FAILURE;
    }

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
            printf("\nSe ha creado un proceso hijo con el PID: %d", pid);

            // El proceso padre no necesita el socket del hijo, lo cierra:
            close(client_fd);
        }
    }
    
    close(server_fd);
    return EXIT_SUCCESS;
}