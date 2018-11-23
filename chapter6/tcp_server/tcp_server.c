#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>


#define TCP_PORT  5100

int main(int argc, char **argv)
{
	int ssock; //socket discript set
	socklen_t clen;
	int n;
	struct sockaddr_in client_addr, server_addr; //address struct set
	char buf[BUFSIZ] = "Hello World";
	
	//server socket create
	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		return -1;
	}
	
	//address struct setting
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(TCP_PORT);
	
	//bind function use and set socket address
	if(bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind()");
		return -1;
	}
	
	//deal stand by queue
	if(listen(ssock, 8) < 0)
	{
		perror("listen()");
		return -1;
	}

	clen = sizeof(client_addr);

	while(1)
	{
		//client connect allow connection and make client socket
		int csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);

		printf("Client is connected %s\n", inet_ntoa(client_addr.sin_addr));

		if((n = read(csock, buf, BUFSIZ)) <= 0)
		{
			perror("read()");
		}

		printf("Received data : %s\n", buf);
		//client buf send string
		if(write(csock, buf, n) <= 0)
		{
			perror("write()");
		}

		close(csock);
	}
	close(ssock);

	return 0;
}


