CC = gcc

TARGET = rpi6

REMOVE = rm -rf

SRC = rpi6.c

CFLAG = -o

INC = -lwiringPi -lpthread

all:
	$(CC) $(CFLAG) $(TARGET) $(SRC) $(INC)

clean:
	$(REMOVE) $(TARGET)