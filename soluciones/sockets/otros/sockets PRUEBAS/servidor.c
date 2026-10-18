// SERVIDOR:


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 8888
#define NUM_CON 10


int main(){
	int sock_s, sock_c;
	struct sockaddr_in dirServidor;
	struct sockaddr_in dirCliente;
	int tam;
	
	// crear el socket del Servidor:
	if ((sock_s = socket(AF_INET, SOCK_STREAM, 0))==-1){
		puts("Se ha producido un error al crear el socket");
		exit(1);
	}
	puts("Socket del servidor creado ...");
	
	// Configurar la direccion del Servidor:
	dirServidor.sin_family = AF_INET;
	dirServidor.sin_port = htons(PUERTO);
	dirServidor.sin_addr.s_addr = INADDR_ANY; // Mi direccion IP
	memset(&dirServidor.sin_zero, '\0', 8);   // El resto de la struct se rellena con ceros:
	
	// Llamar a la funcion bind para ligar el socket con la dir IP, puerto, etc.
	if (bind(sock_s, (struct sockaddr *) &dirServidor, sizeof(struct sockaddr_in))==-1){
		puts("Se ha producido un error al hacer bind");
		exit(2);
	}
	puts("bind Ok ...");
	
	// Configurar el numero maximo de peticiones en la cola:
	if (listen(sock_s, NUM_CON)==-1){
		puts("Se ha producido un error al configurar el numero de conexiones");
		exit(3);
	}
	puts("Listen Ok ...");
	
	// Esperar peticiones de los clientes:
	while (1){
		tam = sizeof(struct sockaddr_in);
		
		puts("Servidor ok, a la espera de clientes ...");
		
		// Llamamos a accept para esperar las conexiones de los clientes, es bloqueante:
		// Nos devuelve un socket del cliente:
		if ((sock_c = accept(sock_s, (struct sockaddr *) &dirCliente, &tam))==-1){
			puts("Error en accept");
			continue; // No sale del bucle, espera a otro cliente.
		}
		printf("\nServidor conectado con %s", inet_ntoa(dirCliente.sin_addr));
		
		// Se desdobla para crear otro proceso, con el hilo y poder seguir atendiendo otros clientes:
		// SE DUPLICAN LAS VARIABLES.
		if (!fork()){
			// Es el proceso hijo:
			close(sock_s); // El hijo no necesita el socket del padre
			if (send(sock_c, "cadena desde el servidor", strlen("cadena desde el servidor"),0)==-1){
				puts("Error al enviar la cadena al hijo");
			}
			close(sock_c); // cierra el socket del cliente.
			exit(0);
		}
		close(sock_c);		
	}
	return 0;
	
	
}
