#include <wiringPi.h>
#include <stdio.h>

#define SW   5
#define CDS  0
#define LED  1



int cdsControl()
{
    int i;
    
    pinMode(SW, INPUT);
    pinMode(CDS, INPUT);
    pinMode(LED, OUTPUT);

    for(i = 0; i < 10000000; i++)
    {
        if(digitalRead(CDS) == HIGH)
        {
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
        }
    }

    return 0;
}

int main(int argc, char ** argv)
{
    wiringPiSetup();
    cdsControl();

    return 0;
}