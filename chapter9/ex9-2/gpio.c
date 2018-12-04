#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[BUFSIZ];
    char i = 0;
    int fd = -1;
    memset(buf, 0, BUFSIZ);

    printf("GPIO Set : %s\n", argv[1]);
    fd = open("/dev/gpioled", O_RDWR);   //gpio device file open

    write(fd, argv[1], strlen(argv[1]), NULL);  //kernel module write

    read(fd, buf, strlen(argv[1]), NULL);    //kernel read data

    printf("Read data : %s\n", buf);   //read data print

    close(fd);           //using data endclose file

    return 0;
}