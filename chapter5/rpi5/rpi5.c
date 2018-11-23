#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softTone.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>


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

pthread_mutex_t music_lock;
pthread_mutex_t motor_lock;

static int is_run = 1; //스레드 종료를 위한 플레이그

void *musicfunction(void *arg)
{
    if(pthread_mutex_trylock(&music_lock) != EBUSY) //임계구역 설정
    {
        musicPlay();
        pthread_mutex_unlock(&music_lock); //임계 구역 해제
    }
    return NULL;
}

void *motorfunction(void *arg)
{
    if(pthread_mutex_trylock(&motor_lock) != EBUSY) //임계 구역 설정
    {
        motorControl(MOTOR);
        pthread_mutex_unlock(&motor_lock); //임계 구역 해제
    }

    return NULL;
}

int ledControl(int gpio, int onoff)
{
    pinMode(gpio, OUTPUT); 
    digitalWrite(gpio, (onoff)?HIGH:LOW); //led 켜고 끄기

    return 0;
}

void *switchfunction(void *arg)
{
    pinMode(SW, INPUT); //핀의 모드를 입력으로 설정
    pinMode(LED, OUTPUT);

    while(is_run)
    {
        if(digitalRead(SW) == LOW) //push버튼이 눌러지면
        {
            digitalWrite(LED, HIGH); //led켜기
        }else{
            digitalWrite(LED, LOW); //led끄기
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

void *cdsfunction(void *arg)
{
    pinMode(SW, INPUT);
    pinMode(CDS, INPUT);
    pinMode(LED, OUTPUT);

    while(is_run)
    {
        if(digitalRead(CDS) == HIGH) //빛이 감지되면(HIGH)
        {
            digitalWrite(LED, HIGH); //LED 켜기
            delay(100);
        }else{
            digitalWrite(LED, LOW); //LED 끄기
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

int kbhit()
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

int main(int argc, char **argv)
{
    int i = 0;
    pthread_t ptSwitch, ptCds, ptMusic, ptMotor;
    pthread_mutex_init(&music_lock, NULL); //뮤텍스 초기화
    pthread_mutex_init(&motor_lock, NULL);

    printf("l : LED On, L : LED Off, m : Music, d : DC Motor, q : Quit\n");

    wiringPiSetup();
    pthread_create(&ptSwitch, NULL, switchfunction, NULL);
    pthread_create(&ptCds, NULL, cdsfunction, NULL);

    wiringPiSetup();
    for(i = 0; ; i++)
    {
        if(kbhit())//키보드가 눌러졌는지 확인한다.
        {
            switch(getchar()) //문자를 읽는다.
            {
                case 'l':
                ledControl(LED, 1); //읽는 문자가 l 이면 led를 켠다.
                break;
                case 'L':
                ledControl(LED, 0); //읽는 문자가 L 이면 led를 끈다.
                break;
                case 'm':
                pthread_create(&ptMusic, NULL, musicfunction, NULL);
                break;
                case 'd':
                pthread_create(&ptMotor, NULL, motorfunction, NULL); //읽은 문자가 d이면 dc motor를 작동한다.
                break;
                case 'q':
                goto END; //읽은 문자가 q이면 종료한다.
                break;
            }
        }
        printf("%20d\t\t\r", i); //현재 카운트한 숫자를 출력한다.
        delay(100);
    }
    END:
    printf("Good Bye !\n");

    is_run = 0;

    pthread_join(ptSwitch, NULL); //스레드 대기
    pthread_join(ptCds, NULL);
    pthread_join(ptMusic, NULL); //스레드 대기
    pthread_join(ptMotor, NULL);

    pthread_mutex_destroy(&music_lock); //뮤텍스 삭제
    pthread_mutex_destroy(&motor_lock);

    return 0;
}