#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int ledControl(int gpio)
{
    int fd;
    char buf[BUFSIZ];
    //gpio를 사용하기 위해 정의된 파일 위치
    //gpio 사용 준비
    fd = open("/sys/class/gpio/export", O_WRONLY);
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);
    //input, output 설정
    //방향 설정
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    //led를 위해 디바이스를 출력으로 설정: 입력의 경우 write(fd, "in", 3);사용
    write(fd, "out", 4);
    close(fd);
    //디바이스에 값 출력
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);
    write(fd, "1", 2);
    close(fd);

    //led 점멸을 위한 delay
    getchar();
    //사용한 gpio 디바이스 해제 하기 
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}

int main(int argc, char **argv)
{
    int gno;

    if(argc < 2)
    {
        printf("Usage : %s GPIO_NO\n", argv[0]);
        return -1;
    }
    gno = atoi(argv[1]);
    ledControl(gno);
    
    return 0;
}