#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "common.h"

int main(int argc , char *argv[])
{
	char client_message[MAX_MESSAGE_SIZE] = { 0 };
	int socket_fd = 0, read_size = sizeof(client_message);
	struct hostent *clientent = NULL;
	int retval = NO_ERROR;
	socklen_t client_socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons( 8888 )
	};
	struct sockaddr_in client;

	socket_fd = socket(AF_INET , SOCK_DGRAM , 0);
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

	// Receive a message from client
	while (!retval && read_size) {
		read_size = recvfrom(socket_fd, client_message, sizeof(client_message), 0,
				(struct sockaddr *)&client, &client_socklen);
		if (read_size < 0) {
			perror("recv failed");
			retval = ERROR_CANT_READ;
			break;
		}

		clientent = gethostbyaddr((const char *)&client.sin_addr.s_addr,
			sizeof(client.sin_addr.s_addr), AF_INET);
		if (clientent == NULL) {
			perror("gethostbyaddr failed");
			retval = ERROR_CANT_GET_HOST_ADDR;
			break;
		}

		fprintf(stderr, "Sending echo to %s (%s)\n", clientent->h_name,
				inet_ntoa(client.sin_addr));
		if (sendto(socket_fd, client_message, strlen(client_message), 0,
				(struct sockaddr *)&client, client_socklen) < 0) {
			perror("can't send data");
			retval = ERROR_CANT_SEND;
			break;
		}
	}

	close(socket_fd);
	return retval;
}
