#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>        //getpwuid():uid를 이용해서 사용자의 이름 구하기
#include <grp.h>        // getgrgid(): gid를 이용해서 그룹의 이름 구하기
#include <time.h>       //localtime()함수
#include <sys/stat.h>
#include <sys/types.h>

int listDir(char *arg)
{
    DIR *pdir;
    struct dirent *dirt;
    struct stat statBuf;
    int i = 0, count = 0;
    char *dirName[255], buf[255], permission[11], mtime[20];
    struct passwd *username;
    struct group *groupname;
    struct tm *t;

    memset(dirName, 0, sizeof(dirName));
    memset(&dirt , 0, sizeof(dirt));
    memset(&statBuf, 0, sizeof(statBuf));

    if((pdir = opendir(arg)) <= 0)
    {
        perror("opendir");
        return -1;
    }
    chdir(arg);
    getcwd(buf, 255);
    printf("\n%s: Directory\n", arg);

    while((dirt = readdir(pdir)) != NULL)
    {
        //현재 디렉터리에 대한 정보 가져오기
        lstat(dirt->d_name, &statBuf);
        //디렉터리인지 파일인지 검사
        if(S_ISDIR(statBuf.st_mode))
        {
            permission[0] = 'd';
        }else if(S_ISLNK(statBuf.st_mode))
        {
            permission[0] = 'l';
        }else if(S_ISCHR(statBuf.st_mode))
        {
            permission[0] = 'c';
        }else if(S_ISBLK(statBuf.st_mode))
        {
            permission[0] = 'b';
        }else if(S_ISSOCK(statBuf.st_mode))
        {
            permission[0] = 's';
        }else if(S_ISFIFO(statBuf.st_mode))
        {
            permission[0] = 'p';
        }else{
            permission[0] = '-';
        }
        // 사용자 권한 검사
        permission[1] = statBuf.st_mode & S_IRUSR ? 'r' : '-';
        permission[2] = statBuf.st_mode & S_IWUSR ? 'w' : '-';
        permission[3] = statBuf.st_mode & S_IRUSR ? 'x' :
                        statBuf.st_mode & S_ISUID ? 'S': '-';
        //그룹 권한 검사
        permission[4] = statBuf.st_mode & S_IRGRP ? 'r' : '-';
        permission[5] = statBuf.st_mode & S_IWGRP ? 'w' : '-';
        permission[6] = statBuf.st_mode & S_IXGRP ? 'x' :
                        statBuf.st_mode & S_ISGID ? 'S': '-';

        //일반 사용자 권한 검사
        permission[7] = statBuf.st_mode & S_IROTH ? 'r' : '-';
        permission[8] = statBuf.st_mode & S_IRUSR ? 'w' : '-';
        permission[9] = statBuf.st_mode & S_IXOTH ? 'x' : '-';

        if(statBuf.st_mode & S_IXOTH)
        {
            permission[9] = statBuf.st_mode & S_ISVTX ? 't' : 'x';
        }else{
            permission[9] = statBuf.st_mode & S_ISVTX ? 'T' : '-';
        }

        permission[10] = '\0';

        if(S_ISDIR(statBuf.st_mode) == 1)
        {
            if(strcmp(dirt->d_name, ".") && strcmp(dirt->d_name, ".."))
            {
                dirName[count] = dirt->d_name;
                count = count + 1;
            }
        }

        username = getpwuid(statBuf.st_uid); //uid에서 사용자의 이름 획득
        groupname = getgrgid(statBuf.st_gid); //gid에서 그룹의 이름 획득
        t = localtime(&statBuf.st_mtime); //최근 수정된 시간 출력
        //출력을 위한 서식화 : tm 구조체는 뒤에서 설명
        sprintf(mtime, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

        printf("%s %2d %s %s %9ld %s %s\n", permission, statBuf.st_nlink, username->pw_name, groupname->gr_name, statBuf.st_size, mtime, dirt->d_name);

    }
    for(i = 0; i < count; i++)
    {
        if(listDir(dirName[i]) == -1)
        {
            break;
        }
    }
    printf("\n");
    closedir(pdir);
    chdir("..");
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage : %s Directory_name.\n", argv[0]);
        return -1;
    }
    listDir(argv[1]);

    return 0;
}