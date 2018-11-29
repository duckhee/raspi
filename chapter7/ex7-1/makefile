CC = gcc

SRCS = alsapcm.c

TARGET = alsapcm

CFLAGS = -lasound -lm

all:
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)

clean:
	rm -f $(TARGET)