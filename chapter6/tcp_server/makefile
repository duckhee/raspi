CC = gcc

SERVER_TARGET = tcp_server

CLIENT_TARGET = tcp_client

REMOVE = rm -rf

CFLAG = -o

SERVER_SRC = tcp_server.c

CLIENT_SRC = tcp_client.c


all:
	$(CC) $(CFLAG) $(SERVER_TARGET) $(SERVER_SRC)
	$(CC) $(CFLAG) $(CLIENT_TARGET) $(CLIENT_SRC)

clean:
	$(REMOVE) $(SERVER_TARGET)
	$(REMOVE) $(CLIENT_TARGET)
