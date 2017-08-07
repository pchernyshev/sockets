#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"

int main(int argc , char *argv[])
{
	char heartbeat_msg[MAX_MESSAGE_SIZE] = "Heartbeat";
	char reply[MAX_MESSAGE_SIZE] = { 0 };
	int sock_fd = 0;
	int retval = NO_ERROR;
	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = inet_addr("127.0.0.1"),
		.sin_port = htons( 8888 )
	};

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1) {
		perror("Could not create socket");
		return ERROR_CANT_CREATE;
	} else
		fprintf(stderr, "Socket created\n");


	if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("Can't connect");
		retval = ERROR_CANT_CONNECT;
	} else
		fprintf(stderr, "Connected...\n");


	while (!retval && !sleep(1)) {
		if (send(sock_fd, heartbeat_msg, strlen(heartbeat_msg), 0) < 0) {
			perror("Can't send the message");
			retval = ERROR_CANT_SEND;
			break;
		}

		if (recv(sock_fd, reply, sizeof(reply) , 0) < 0) {
			perror("recv failed");
			retval = ERROR_CANT_READ;
			break;
		}

		printf("Got: %.*s\n", (int)sizeof(reply), reply);
	}

	close(sock_fd);
	return retval;
}

