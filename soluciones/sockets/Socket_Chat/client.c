
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define PUERTO 3333 // puerto al que vamos a conectar
#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

int main(int argc, char *argv[]) {
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in dir_Destino; // información de la dirección de destino

	if (argc != 2) {
		fprintf(stderr, "Uso: ./Socket_Client nombre_host (Ejemplo: ./Socket_Client localhost) \n");
		exit(1);
	}

	if ((he = gethostbyname(argv[1])) == NULL) { // obtener información de máquina
		perror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	dir_Destino.sin_family = AF_INET; // Ordenación de bytes de la máquina
	dir_Destino.sin_port = htons(PUERTO); // short, Ordenación de bytes de la red
	dir_Destino.sin_addr = *((struct in_addr *) he->h_addr);
	memset(&(dir_Destino.sin_zero), '\0', 8); // poner a cero el resto de la estructura

	if (connect(sockfd, (struct sockaddr *) &dir_Destino, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	
	char cadena[200];
	

	do {
		printf("\nTeclee el mensaje para el server: ");
		scanf("%s", cadena);
		puts ("\nesperando mensaje");	

		if(send(sockfd, cadena, strlen(cadena), 0) == -1) 
			puts("\nError al enviar cadena");
		else {
			if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
				perror("recv");
				exit(1);
			}

			buf[numbytes] = '\0';
			printf("SERVIDOR RESPONDE: %s", buf);
		}
		

	}while(strcmp(cadena, "fin") != 0); 
	
	close(sockfd);
	return 0;
}

