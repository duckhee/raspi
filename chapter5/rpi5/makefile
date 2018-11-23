CC = gcc

TARGET = rpi5

REMOVE = rm -rf

SRC = rpi5.c

CFLAG = -o

INC = -lwiringPi -lpthread

all:
	$(CC) $(CFLAG) $(TARGET) $(SRC) $(INC)

clean:
	$(REMOVE) $(TARGET)