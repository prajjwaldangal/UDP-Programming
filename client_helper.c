#include <string.h>

#define MAX_LINE    (1000)

char newLineChar = '\n';

char * handleS()
{
	char * cap_str = "CAP";
	char * msg_comp = (char *) malloc(sizeof(char) * MAX_LINE);
	strcat(msg_comp, cap_str);
	strcat(msg_comp, &newLineChar);
	
	char * msg = (char *) malloc(sizeof(char) * MAX_LINE-5);
	printf("Enter the message: \n");
	fgets(msg, MAX_LINE-5, stdin);
	scanf("%s", msg);
	
	strcat(msg_comp, msg);
	strcat(msg_comp, &newLineChar);

	return msg_comp;
}

char * handleT(int port) 
{
	char * file_str = "FILE";
	char * TCP_port_no = malloc(sizeof(char));
	char * msg_comp = (char *) malloc(sizeof(char) * MAX_LINE);
	strcat(msg_comp, file_str);
	strcat(msg_comp, &newLineChar); 

	char * path = (char *) malloc(sizeof(char) * MAX_LINE-6);
	printf("Enter file path: \n");
	fgets(path, MAX_LINE-6, stdin);
	scanf("%s", path);

	strcat(msg_comp, path);
	strcat(msg_comp, &newLineChar); // "FILE\nxxx\nkkk\n"
	sprintf(TCP_port_no, "%i", port);
	strcat(msg_comp, TCP_port_no);

	return msg_comp;
}
