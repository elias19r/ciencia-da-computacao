#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
char *getpage(char *ip, int port, char *page) {
	int sockfd = 0, n = 0, count = 0;
	char *recvBuff = NULL, *sendBuff;
	struct sockaddr_in serv_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return NULL;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return NULL;
	}
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return NULL;
	}
	// requesting the page
	// building the request string
	sendBuff = (char *) malloc(sizeof(char) * (strlen(page) + 7));
	snprintf(sendBuff, sizeof(char) * (strlen(page) + 7), "GET %s\r\n", page);
	// writing in the socket the request
	write(sockfd, sendBuff, strlen(sendBuff));
	// reading the page
	int i = 0;
	count = 1;
	recvBuff = (char *) realloc(recvBuff, sizeof(char));
	// read each character of the page
	while ( (n = read(sockfd, &recvBuff[count-1], 1)) > 0)
	{
		//printf("%c", recvBuff[count]);
		recvBuff = (char *) realloc(recvBuff, sizeof(char) * (count+1));
		count++;
	}
	recvBuff[count-1] = '\0';
	free(sendBuff);
	
	return (recvBuff);
}

int main(int argc, char *argv[])
{
	char *ip, *page, *data;
	int port;
	
	if(argc != 4)
	{
		printf("\n Usage: %s <ip of server> <port> <page>\n",argv[0]);
		return 1;
	}
	
	ip = argv[1];
	port = atoi(argv[2]);
	page = argv[3];
	data = getpage(ip, port, page);
	printf("%s\n", data);
	
	free(data);
	return 0;
}
