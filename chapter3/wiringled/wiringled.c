#include <wiringPi.h>
#include <stdio.h>

int ledControl(int gpio)
{
    int i;
    pinMode(gpio, OUTPUT);

    for(i = 0; i < 5; i++)
    {
        digitalWrite(gpio, HIGH);
        delay(1000);
        digitalWrite(gpio, LOW);
        delay(1000);
    }

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
    //atoi 문자열을 정수형으로 변환 해서 반환 하는 함수 
    gno = atoi(argv[1]);
    wiringPiSetup();
    ledControl();
    return 0;
}