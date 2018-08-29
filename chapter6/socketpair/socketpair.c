#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
    int z, socket_fd[2];
    int status;
    char buf[] = "Hello World", line[BUFSIZ];
    pid_t pid;

    z = socketpair(AF_LOCAL, SOCK_STREAM, 0, socket_fd); //한쌍의 소켓을 생성
    if(z == -1)
    {
        perror("socketpair");
        return -1;
    }

    printf("socket 1: %d\n", socket_fd[0]); //각 소켓의 디스크립트 번호를 출력
    printf("socket 2 : %d\n", socket_fd[1]);

    if((pid = fork()) < 0) //fork() 함수 실행 에러 시의 처리
    {
        perror("fork()");
    }else if(pid == 0) //자식 프로세스일 때 
    {
        write(socket_fd[0],  buf, strlen(buf) + 1); //부모 프로세스로 테이터 보내기
        printf("Data send : %s\n", buf);

        close(socket_fd[0]); //소켓 닫기
    }else{
        wait(&status); //부모 프로세스일 때의 처리
        read(socket_fd[1], line, BUFSIZ); //자식 프로세의 종료 대기
        printf("Received data : %s\n", line); 
        close(socket_fd[1]); 
    }

    return 0;
}
