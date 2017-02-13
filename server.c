#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>     // inet (3) functions
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "helper.h"
#include "helper.c"

char newLineChar = '\n';

#define ECHO_PORT       (2002)
#define MAX_LINE        (1000)
#define LISTENQ			(1024)

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) 
{

	char buffer[MAX_LINE];
	struct sockaddr_in servaddr, clientaddr;
	int list_s, conn_s;
	struct hostent *hostp;
	char *hostaddrp;
	
	// create socket
	list_s = socket(AF_INET, SOCK_DGRAM, 0);

	int optval = 1;
	setsockopt(list_s, SOL_SOCKET, SO_REUSEADDR, 
				(const void *) &optval, sizeof(int));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);

	// bind socket to address
	if (bind(list_s, (struct sockaddr *) &servaddr, 
		sizeof(servaddr)) < 0)
		error("ERROR on binding");

	// bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr));
	socklen_t clientlen = sizeof(clientaddr);
	while (1)
	{
		// accept
		bzero(buffer, MAX_LINE);

		if ((conn_s = recvfrom(list_s, buffer, MAX_LINE, 0,
						(struct sockaddr *) &clientaddr, &clientlen)) < 0)
		{
			error("ERROR in recvfrom serverside");
			break;
		}
    	printf("server received %lu/%d bytes: %s\n", strlen(buffer), conn_s, buffer);
    

		// Readline(conn_s, buffer, MAX_LINE);
		char * cap_str = "CAP";
		char * file_str = "FILE";
		char * ret_str = (char *) malloc(sizeof(char) * MAX_LINE);
		char * semi_buf = (char *) malloc(sizeof(char) * MAX_LINE-5);
		
		printf("Received buffer: %s\n", buffer);

		// related to handling case of t
		FILE * fp;
		char file_path[MAX_LINE-7];
		long bytes;
		char n_bytes[MAX_LINE-3]; // this means we will have limitation as to the size of file
		char file_buf[MAX_LINE-1];

		int n_cap = 0, n_file = 0;

		for (int i=0; i < 3; i++) {
			if (buffer[i] == cap_str[i]) {
				n_cap ++;
			}
			else if (buffer[i] == file_str[i]) {
				n_file ++;
			} 
			else {
				continue;
			}
		}
		printf("n_cap: %d, n_file: %d\n", n_cap, n_file);
		int cap_count;

		// this if block handles s on the server side
		if (n_cap > n_file)
		{
			int i = 4;
			int n_c = 1;
			while (n_c < 2)
			{
				if (buffer[i] == '\n')
				{
					n_c++;
				} else {
					* (semi_buf+ i-4) = toupper(buffer[i]);
					cap_count++;
				}
				i++;	
			}
			printf("str len %lu\n", strlen(buffer));
			// for (int i=0; i < str)

			sprintf(ret_str, "%i", cap_count);  /* converting int to string */
			strcat(ret_str, "\n");
			strcat(ret_str, semi_buf);
			printf("return string %s\n", ret_str);
			printf("semi_buf: %s\n", semi_buf);

		} else 
		// this block handles t on the server side
		{
			for (int i=5; i < strlen(buffer)-1; i++) { // MAX_LINE - 1 to escape reading the last \n
				file_path[i-5] = buffer[i];
			}
			printf("file path: %s\n", file_path);
			fp = fopen(file_path, "r");
			if (fp == NULL) {
				strcat(ret_str, "9");
				strcat(ret_str, &newLineChar);
				strcat(ret_str, "NOT FOUND");
			} else {
				if (fseek(fp, 0, SEEK_END) != 0) {
					printf("Error in seeking to the end of file");
				}

				// ret_str is the buffer, we add all the requird formattin to it
				bytes = ftell(fp);
				printf("File size %ld\n", bytes);
				sprintf(n_bytes, "%ld", bytes);
				strcat(ret_str, n_bytes);
				strcat(ret_str, "\n");
				fclose(fp);
				
				// malloc was the only way I could get it to work
				char * conv2 = malloc(sizeof(char));
				fp = fopen(file_path, "r");
				int c = fgetc(fp);
				sprintf(conv2, "%i", c);
				strcat(ret_str, conv2);

				// Writeline(conn_s, ret_str, MAX_LINE-1);
				list_s = sendto(conn_s, buffer, MAX_LINE, 0,
							(struct sockaddr *) NULL, 0);
				printf("c: %c\n", c);	

				// what I understood for part two on the severside is
				// we could have a very large text file and we would have to 
				// write buffer over buffer until the whole file was read
				c = fgetc(fp);
				while (c != EOF) {
					if (strlen(ret_str) == MAX_LINE-8){
						Writeline(conn_s, ret_str, MAX_LINE-1);
						memset(ret_str, 0, MAX_LINE);
					} else {
						c = fgetc(fp);
						printf("C: %c", c);
						sprintf(conv2, "%d", c);
						strcat(ret_str, conv2);
					}
				}	
				fclose(fp);	
				// break;					
			}
		}
		list_s = sendto(conn_s, ret_str, MAX_LINE, 0,
						(struct sockaddr *) &clientaddr, clientlen);
		// Writeline(conn_s, ret_str, MAX_LINE-1);
		close (conn_s);
	}
}
