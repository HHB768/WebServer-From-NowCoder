#define  _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <wait.h>

void myalarm(int num)
{
    printf("捕捉到的信号编号是：%d\n", num);
    while(1)
    {
        // 若其父进程未提前调用wait
        // 则内核会持续保留子进程的退出状态等信息，以使父进程可以wait获取之
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret > 0)
            printf("child die, pid: %d\n", ret);
        else if(ret == 0)
            break;
        else
            break;
    //    waitpid(): on success, returns the process ID of the child whose state
    //    has changed; if WNOHANG was specified and one or more child(ren) spec‐
    //    ified by pid exist, but have not yet changed  state,  then  0  is  re‐
    //    turned.  On error, -1 is returned.

    }
    wait(NULL);  // 嘿嘿接受不来，很多信号被丢了
}

int main()
{
    // 提前设置好阻塞
    // 因为可能子进程很快结束，父进程还没注册好捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t pid;
    for(int i=0; i<20; i++)
    {
        pid = fork();
        if(pid == 0)
            break;
    }

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);

    if(pid > 0)
    {
        sigaction(SIGCHLD, &act, NULL);
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        while(1)
        {
            // 父进程正常业务逻辑
            printf("parent process, pid:%d\n", getpid());
            sleep(2);
        }
    }
    else if(pid == 0)
    {
        printf("child process, pid:%d\n", getpid());
    }

    return 0;
}