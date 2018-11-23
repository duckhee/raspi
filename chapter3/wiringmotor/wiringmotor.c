#include <wiringPi.h>
#include <stdio.h>

int motorControl(int gpio)
{
    int i;
    pinMode(gpio, OUTPUT);

    for(i = 0; i < 3; i++)
    {
        digitalWrite(gpio, HIGH);
        delay(1000);
        digitalWrite(gpio, LOW);
        delay(1000);
    }
    return 0;
}

int main(int argc, char** argv)
{
    int gno;
    
    if(argc < 2)
    {
        printf("Usage : %s GPIO_NO\n", argv[0]);
        return -1;
    }
    gno = atoi(argv[1]);

    wiringPiSetup();

    motorControl(gno);

    return 0;
}