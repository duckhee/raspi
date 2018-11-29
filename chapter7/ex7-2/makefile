CC = gcc

SRCS = alsavolume.c

TARGET = alsavolume

CFLAGS = -lasound -lm

all:
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)

clean:
	rm -f $(TARGET)