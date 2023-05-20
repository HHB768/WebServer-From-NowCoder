// 写一个守护进程，每隔几秒获取一下时间并写入磁盘
#define  _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


void work(int sigid)
{
    time_t tm = time(NULL);
    struct tm * loc = localtime(&tm);
//     char buf[1024];
//     sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year, loc->tm_mon, loc->tm_mday,
//     loc->tm_hour, loc->tm_min, loc->tm_sec);
//     printf("%s\n", buf);
    char * str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);
}

int main()
{
    // 1
    pid_t pid = fork();
    if(pid>0)
        exit(0);
    
    // 2
    setsid();

    // 3
    umask(022);

    // 4
    chdir("~/linux/l28");

    // 5
    int fd = open("dev/null", O_RDWR);
    dup2(fd, STDERR_FILENO);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);

    // 6
    struct sigaction act;

    act.sa_flags= 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    struct itimerval val;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &val, NULL);

    while(1){sleep(10);};

    return 0;
}