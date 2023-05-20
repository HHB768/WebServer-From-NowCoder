#define  _DEFAULT_SOURCE
#include <signal.h>

// int sigaction(int signum, const struct sigaction *act,
//                 struct sigaction *oldact);
// signum: 信号编号
// act: 捕捉动作之后的处理动作
// oldact: 上一次相关设置，一般不用，传递NULL
// return 0 / -1

// struct sigaction {
//     void     (*sa_handler)(int);  // 回调函数
//     void     (*sa_sigaction)(int, siginfo_t *, void *);  // 不常用
//     sigset_t   sa_mask;  // 临时阻塞信号集，信号处理完后回到系统那个
//     int        sa_flags;  // 0则使用sa_handler，SA_SIGINFO则使用sa_sigaction
//     void     (*sa_restorer)(void);  // 不用了
// };


#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

void myalarm(int num)
{
    printf("捕捉到的信号编号是：%d\n", num);
    // 常规的信号阻塞时是不支持排队的，未决信号置1，在信号处理结束之前，同一个信号都丢掉
    sleep(5);
}

int main()
{
    // 注册好信号捕捉
    // signal(SIGALRM, SIG_IGN);
    // signal(SIGALRM, SIG_DFL;

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;  
    // 实际上是 act.__sigaction_handler.sa_handler，但是有宏定义
    sigemptyset(&act.sa_mask);

    // sigaction 在各个平台的实现比较统一，建议用它代替signal
    sigaction(SIGALRM, &act, NULL);
    // __sighandler_t res = signal(SIGALRM, myalarm);
    

    // if(res == SIG_ERR)
    // {
    //     perror("signal");
    //     exit(0);
    // }

    // 过三秒后每两秒执行一次
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);  // 非阻塞
    printf("计时开始\n");

    if(ret == -1)
    {
        perror("setitimer");
        exit(0);
    }

    while(1) {}

    return 0;


}

