CC = gcc

SERVERTARGET = msg_server

CLIENTTARGET = msg_client

REMOVE = rm -rf

CFLAG = -o

SERVERSRC = msg_server.c

CLIENTSRC = msg_client.c

all:
	$(CC) $(CFLAG) $(SERVERTARGET) $(SERVERSRC)
	$(CC) $(CFLAG) $(CLIENTTARGET) $(CLIENTSRC)


clean:
	$(REMOVE) $(SERVERTARGET)
	$(REMOVE) $(CLIENTTARGET)



