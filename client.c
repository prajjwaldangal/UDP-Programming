#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <errno.h>

#include "helper.h"
#include "helper.c"

#include "client_helper.c"

#define ECHO_PORT       (2002)
#define MAX_LINE		(1000)

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main (int argc, char *argv[]) // should start from 1 -> argv
{
	// bunch of initializations
	char buffer[MAX_LINE];
	struct sockaddr_in servaddr;
	short int port;                  /*  port number               */
	char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;
    char choice;
	char * msg = (char *) malloc (sizeof(char) * MAX_LINE);

	// memset is required to get the connection going
	printf("Enter s, t, or q (lowercase): ");
	scanf("%s", &choice);
	switch (choice)
	{
		case 's':
			msg = handleS();
			break;
		case 't':
			msg = handleT();
			break;
		case 'q':
			return 0;
			break;
		default:
			break;
	}
	int conn_s;
	// create socket
	if ((conn_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		error("Error opening socket \n");
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;			     
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);
	
	strcpy(buffer, msg);

	int serverlen = sizeof(servaddr);
	int n = sendto(sockfd, buffer, MAX_LINE, 0, &servaddr, &serverlen);
	if (n < 0) 
		error("ERROR in sendto");
	n = recvfrom(conn_s, buffer, MAX_LINE, 0, &servaddr, &serverlen);
	if (n < 0)
		error("ERROR in recvfrom");
	printf("Receive buffer %s\n", buffer);
	
	return 0;
}