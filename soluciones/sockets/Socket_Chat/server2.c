
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PUERTO 3333    // Puerto al que conectarán los usuarios
#define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola
#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

int main(void) {
	char buf[MAXDATASIZE];
	int sockfd, new_fd; // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
	struct sockaddr_in dir_Servidor; // información sobre mi dirección
	struct sockaddr_in dir_Cliente; // información sobre la dirección del cliente
	int sin_size;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	dir_Servidor.sin_family = AF_INET;			// Ordenación de bytes de la máquina
	dir_Servidor.sin_port = htons(PUERTO);		// short, Ordenación de bytes de la red
	dir_Servidor.sin_addr.s_addr = INADDR_ANY;	// Rellenar con mi dirección IP
	memset(&(dir_Servidor.sin_zero), '\0', 8);	//Poner a cero el resto de la estructura


	if (bind(sockfd, (struct sockaddr *)&dir_Servidor, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	puts("Servidor a la espera de clientes.");

	while (1) { // main accept() loop
		sin_size = sizeof(struct sockaddr_in);
		puts("esperando conexion:");

		if ((new_fd	= accept(sockfd, (struct sockaddr *) &dir_Cliente, &sin_size))== -1) {
			puts("error en accept");
			continue;
		}
		printf("Servidor: Conexion establecida con %s\n", inet_ntoa(dir_Cliente.sin_addr));

		// Este es el proceso hijo
		
		int numbytes;
		strcpy(buf, "");

		do  {
		
			puts("\nEsperando mensaje del cliente:");
			if ((numbytes = recv(new_fd, buf, MAXDATASIZE - 1, 0)) == -1) {
				perror("\n error lectura en el servidor.");
				exit(1);
			}

			buf[numbytes] = '\0';
			strcat(buf, " SERVER");
			
			printf("envio respuesta desde el servidor: %s", buf);
			if (send(new_fd, buf, strlen(buf), 0) == -1)
				perror("send");

		}  while(strcmp(buf, "fin") != 0);
		close(new_fd);
		puts("\nCliente desconectado ...");
			
	}
	
	return 0;
}



