CC = gcc

TARGETSERVER = fifo_server

TARGETCLIENT = fifo_client

REMOVE = rm -rf

CFLAG = -o

SERVERSRC = fifo_server.c

CLIENTSRC = fifo_client.c

all:
	$(CC) $(CFLAG) $(TARGETSERVER) $(SERVERSRC)
	$(CC) $(CFLAG) $(TARGETCLIENT) $(CLIENTSRC)


clean:
	$(REMOVE) $(TARGETCLIENT)
	$(REMOVE) $(TARGETSERVER)

