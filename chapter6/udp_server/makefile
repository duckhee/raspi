
CC = gcc

REMOVE = rm -rf

SERVER_TARGET = udpserver

CLIENT_TARGET = udpclient

CFLAG = -o

SERVER_SRC = udp_server.c

CLIENT_SRC = udp_client.c


all:
	$(CC) $(CFLAG) $(SERVER_TARGET) $(SERVER_SRC)
	$(CC) $(CFLAG) $(CLIENT_TARGET) $(CLIENT_SRC)

clean:
	$(REMOVE) $(SERVER_TARGET)
	$(REMOVE) $(CLIENT_TARGET)


