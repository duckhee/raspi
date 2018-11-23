#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softTone.h>


#define CDS     0 //gpio17
#define LED     1 //gpio18
#define SW      5 //gpio24
#define SPKR    6 //gpio25
#define MOTOR   2 //gpio13
#define TOTAL   32

int notes[] = {
    391, 931, 440, 440, 391, 391, 329.63, 329.63, \
    391, 391 329.63, 329.63, 293.66, 293.66, 293.63, 0, \
    391, 391, 440, 440, 391, 391, 329.63, 329.63, \
    391, 329.63, 293.63, 293.66, 329,63, 261.63, 261.63, 261.63, 0
};

int ledControl(ubt gpio, int onoff)
{
    pinMode(gpio, OUTPUT); 
    digitalWrite(gpio, (onoff)?HIGH:LOW); //led 켜고 끄기

    return 0;
}

int switchControl()
{
    int i;

    pinMode(SW, INPUT);
    pinMode(LED, OUTPUT);

    for(i = 0; i < 10000000; i++)
    {
        if(digitalWrite(SW) == LOW)
        {
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
            delay(1000);
        }
    }
    return 0;
}

int musicPlay()
{
    int i;
    softToneCreate(SPKR);
    for(i = 0; i < TOTAL; ++i)
    {
        softToneWrite(SPKR, notes[i]);
        delay(200);
    }
    return 0;
}

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
            ditialWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
        }
    }
    return 0;
}

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

int kdhit()
{
    struct termios oldt, newt; //터미널에 대한 구조체
    int ch, oldf;

    tcgetattr(0, &oldt); //현재 터미널에 설정된 정보를 가져온다.
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); //정규 모드 입력과 에코를 해제한다.
    tcsetattr(0, TCSANOW, &newt); //새로 값으로 터미널을 설정한다.
    oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK); //입력을 넌블로킹 모드로 설정한다.

    ch = getchar();

    tcsetattr(-, TCSANOW, &oldt); //기존의 값으로 터미널의 속성을 바로 적용한다.
    fcntl(0, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin); //앞에서 읽은 위치로 이전으로 포인터를 돌려준다.
        return 1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    int i = 0;
    printf("l : LED On, L : LED Off, s : Switch, m : Music, c : Light Sensor, d : DC Motor, q : Quit\n");

    wiringPiSetup();
    for(i = 0;; i++)
    {
        if(kbhit()) //키보드가 눌렸는지 확인한다.
        {
            switch(getchar()) //문자를 읽는다.
            {
                case 'l':
                ledControl(LED, 1);
                break;
                case 'L':
                ledControl(LED, 0);
                break;
                case 's':
                switchControl();
                break;
                case 'm':
                musicPlay();
                break;
                case 'c':
                cdsControl();
                break;
                case 'd':
                motorControl(MOTOR);
                break;
                case 'q':
                goto END;
                break;
            }
        }
        printf("%20d\t\t\r", i); //현재 카운트 숫자를 출력한다.
        delay(100);
    }
    END:
    printf("GOOD BYE!\n");
    return 0;
}