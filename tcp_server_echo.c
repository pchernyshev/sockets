#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"

int main(int argc , char *argv[])
{
	char client_message[MAX_MESSAGE_SIZE] = { 0 };
	int socket_fd = 0, client_sock = 0, read_size = sizeof(client_message);
	int retval = NO_ERROR;
	socklen_t client_socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons( 8888 )
	};
	struct sockaddr_in client;

	socket_fd = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_fd == -1) {
		perror("Could not create socket");
		return ERROR_CANT_CREATE;
	} else
		fprintf(stderr, "Socket created\n");


	if (bind(socket_fd,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("bind failed");
		retval = ERROR_CANT_BIND;
	} else
		fprintf(stderr, "Waiting for incoming connections...\n");


	if (!retval)
		if (listen(socket_fd , 3))
			retval = ERROR_CANT_LISTEN;


	if (!retval) {
		client_sock = accept(socket_fd, (struct sockaddr *)&client, &client_socklen);
		if (client_sock < 0) {
			perror("accept failed");
			retval = ERROR_CANT_ACCEPT;
		} else
			fprintf(stderr, "Connection accepted\n");
	}


	// Receive a message from client
	while (!retval && read_size) {
		read_size = recv(client_sock, client_message, sizeof(client_message) , 0);
		switch (read_size) {
			case 0:
				fprintf(stderr, "Client disconnected\n");
				break;
			case -1:
				perror("recv failed");
				retval = ERROR_CANT_READ;
				break;
			default:
				write(client_sock, client_message, strlen(client_message));
				break;
		}
	}

	close(socket_fd);
	return retval;
}
