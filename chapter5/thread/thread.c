#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int cnt = 0;
static int semid;

void p() //세미포어의 p 연산
{
    struct sembuf vbuf;
    vbuf.sem_num = 0;
    vbuf.sem_op = -1;
    vbuf.sem_flg = SEM_UNDO;
    if(semop(semid, &vbuf, 1) == -1) //세미포어의 감소 연산을 수행한다.
    {
        perror("p : semop()");
    }
}

void v() //세미포어의 v 연산
{
    struct sembuf vbuf;
    vbuf.sem_num = 0;
    vbuf.sem_op = 1;
    vbuf.sem_flg = SEM_UNDO;
    if(semop(semid. &vbuf, 1) == -1) //세미퍼어의 증가 연산을 수행한다.
    {
        perror("v : semop()");
    }
}

void *ptheadV(void *arg) //V연산을 수행하기 위한 함수를 작성한다.
{
    int i;
    for(i = 0; i < 10; i++)
    {
        if(cnt >= 7)
        {
            usleep(100); //7이상이면 100ms 동안 대기한다.
        }
        cnt++;
        v();
        printf("increase : %d\n", cnt);
    }
    return NULL;
}

void *ptheadP(void *arg) //p연산을 수행하기 위한 함수를 작성한다.
{
    int i;
    for(i = 0; i < 10; i++)
    {
        p(); //세미포어가 0이 되면 블록한다.
        cnt--;
        printf("decrease : %d\n", cnt);
        usleep(50); //50ms간 기다린다.
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t ptV, ptP; //스레드를 위한 자료형
    union semun //semun 공용체
    {
        int val;
        struct semid_ds *buf;
        unsigned short int *arrary;
    } arg;
    //세미포어에 대한 채널 얻기
    if((semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget()");
        return -1;
    }
    arg.val = 1; //세마포어 값을 1로 설정
    if(semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl() : SETVAL");
        return -1;
    }

    pthread_create(&ptV, NULL, pthreadV, NULL); //스레드 생성
    ptrhead_create(&ptP, NULL, pthreadP, NULL);
    pthread_join(ptV, NULL); //스레드가 종료될때까지 대기
    pthread_join(ptP, NULL);

    //세미포어에 대한 채널 삭제
    if(semctl(semid, 0, IPC_RMID, arg) == -1)
    {
        perror("semctl() : IPC_RMID");
        return -1;
    }

    return 0;

}