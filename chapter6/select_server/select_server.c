#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define SERVER_PORT 5100

int main(int argc, char** argv)
{
	int ssock;
	socklen_t clen;
	int n;
	struct sockaddr_in server_addr, client_addr;
	char buf[BUFSIZ] = "Hello World";

	//select() function struct
	fd_set readfd;
	int maxfd, client_index, start_index;
	//client socket fd array
	int client_fd[5] = {0};

	//server socket discript open
	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		return -1;
	}
	//os set service
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	if(bind(ssock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind()");
		return -1;
	}

	if(listen(ssock, 8) < 0)
	{
		perror("listen()");
		return -1;
	}

	//fd_set all of zero set
	FD_ZERO(&readfd);
	//max server socket discripter set??
	maxfd = ssock;
	client_index = 0;
	
	while(1)
	{
		//read function recon set
		FD_SET(ssock, &readfd);
		for(start_index = 0; start_index < client_index; start_index++)
		{
			FD_SET(client_fd[start_index], &readfd);
			if(client_fd[start_index] > maxfd)
			{
				maxfd = client_fd[start_index];
			}
		}
		maxfd = maxfd+1;
		//select() function readable possible search
		select(maxfd, &readfd, NULL, NULL, NULL); //readable ready before block
		if(FD_ISSET(ssock, &readfd)) //readable socket is server socket
		{	
			clen = sizeof(struct sockaddr_in);
			//client request accept
			int csock = accept(ssock, (struct sockaddr*)&client_addr, &clen);
			if(csock < 0)
			{
				perror("accept Error\n");
				return -1;
			}else{
				//new client socket number add fd_set
				FD_SET(csock, &readfd);
				client_fd[client_index] = csock;
				client_index++;
				continue;
			}
			if(client_index == 5)
			{
				break;
			}
		}
		//if readable possible socket
		for(start_index = 0; start_index < client_index; start_index++)
		{
			//use for loop search all client socket
			if(FD_ISSET(client_fd[start_index], &readfd))
			{
				bzero(buf, sizeof(buf));
				
				if((n = read(client_fd[start_index], buf, sizeof(buf))) > 0)
				{
					printf("Received data :%s", buf);
					write(client_fd[start_index], buf, n);
					close(client_fd[start_index]);
					FD_CLR(client_fd[start_index], &readfd);
					client_index--;
				}
			}
		}
	}

	close(ssock);

	return 0;
}

