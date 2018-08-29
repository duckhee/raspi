#include <wiringPi.h>
#include <stdio.h>
#include <softTone.h>

#define SPKR 6   //gpio 25
#define TOTAL 32

int note[] = {
    391, 391, 440, 440, 391, 391, 329.63, 329.63, \
    391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
    391, 391, 440, 440, 391, 391, 329.63, 329.63, \
    391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};

int musicPlay()
{
    int i;
    softToneCreate(SPKR); //ton 출력을 위한 GPIO 설정

    for(i = 0; i < TOTAL; ++i)
    {
        softTonWrite(SPKR, note[i]);
        delay(280);
    }
    return 0;
}

int main(int argc, char **argv)
{
    wiringPiSetup();
    musicPlay();
    return 0;
}