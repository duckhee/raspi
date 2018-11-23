#include <stdio.h>
#include <signal.h> //signal 함수 사용을 위해
#include <stdlib.h> //exit 함수 사용을 위해
#include <unistd.h>
#include <sys/types.h>

static void printSigset(sigset_t *set); //현재 sigset_t에 설정된 시그널 표시
static void sigHandler(int); //시그널 처리용 핸들러

int main(int argc, char** argv)
{
    sigset_t pset; //블록할 시그널을 등록할 sigset_t 형
    sigemptyset(&pset); //모두 0으로 설정
    sigaddset(&pset, SIGOUIT); //sigquit와 sigrtmin을 설정
    sigaddset(&pset, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &pset, NULL); //현재의 시그널 마스크에 추가

    printSigset(&pset); //현재 설정된 sigset_t를 화면으로 출력

    if(signal(SIGINT, sigHandler) == SIG_ERR) //sigint의 처리를 위한 핸들러 등록
    {
        perror("signal() : SIGINT");
        return -1;
    }
    if(signal(SIGUSR1, sigHandler) == SIG_ERR) //sigusr1 처리를 위한 핸들러 등록
    {
        perror("signal() : SIGUSR1");
        return -1;
    }
    if(signal(SIGUSR2, sigHandler) == SIG_ERR) //siguser2 처리를 위한 핸들러 등록
    {
        perror("signal() : SIGUSR2");
        return -1;
    }
    if(signal(SIGIPE, SIG_IGN) == SIG_ERR) //sigpipe 처리를 위한 핸들러 등록
    {
        perror("signal() : SIGPIPE");
        return -1;
    }
    while(1)
    {
        pause(); //시그널 처리를 위한 대기
    }
    return 0;
}

static void sigHandler(int signo)
{
    if(signo == SIGINT)
    {
        printf("SIGINT is catched : %d\n", signo);
        exit(0);
    }else if(signo == SIGUSR1)
    {
        printf("SIGUSR1 is catched\n");
    }else if(signo == SIGUSR2)
    {
        printf("SIGUSR2 is catched\n");
    }else if(signo == SIGQUIT)
    {
        printf("SIGQUIT is catched\n");
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset, SIGINT);
        sigprocmask(SIG_UNBLOCK, &uset, NULL);
    }else{
        fprintf(stderr, "Catched signal : %s\n", strsignal(signo));
    }
}

static void printSigset(sigset_t *set)
{
    int i ;
    for(i = 1; i < NSIG; ++i)
    {
        printf((sigismemeber(set, i))?"1":"0");
    }
    putchar('\n');
}