#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

// int kill(pid_t pid, int sig);
// The  kill()  system call can be used to send any signal to any process
// group or process. return 0 or -1.

// pid > 0: 将信号发送给指定进程
//    == 0：发送给组
//    == -1：发送给每一个有权接受这个信号的进程
//    <  -1：发送给进程组 gid = -pid
// sig：信号的编号或宏值（建议，因为不同的实现编号不同）0表示不发送信号

// int raise(int sig);
// The  raise() function sends a signal to the calling process or thread.
// In a single-threaded program it is equivalent to
// kill(getpid(), sig);

// In a multithreaded program it is equivalent to
// pthread_kill(pthread_self(), sig);
// raise() returns 0 on success, and nonzero for failure.

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// void abort(void);
// 发送SIGABRT给当前进程
// The abort() function never returns.

// unsigned int alarm(unsigned int seconds);
// set an alarm clock for delivery of a signal
// 注意如果秒数为0，则不发送信号
// 每个进程都只有一个定时器，alarm调用后之前的定时器被取消
// In any event any previously set alarm() is canceled.

// return : 0, 之前没有定时器
//          ~0, 之前定时器的倒计时

#include <sys/time.h>
// int setitimer(int which, const struct itimerval *new_value,
//                     struct itimerval *old_value);
// set value of an interval timer，可以覆盖alarm功能
// 微秒，周期性计时
// which：ITIMER_REAL，ITIMER_VIRTUAL，ITIMER_PROF
//       真实时间(常用) 用户态CPU时间    总CPU时间  (有人说alarm是这个，包括状态切换)
//       SIGALRM       SIGVTALRM       SIGPROF     （但我感觉像是第一个）
// struct new_value: 
// struct itimerval {
//     struct timeval it_interval; /* Interval for periodic timer */
//     struct timeval it_value;    /* Time until next expiration */
// };

// struct timeval {
//     time_t      tv_sec;         /* seconds */
//     suseconds_t tv_usec;        /* microseconds */
// };

// old_value 一般不用，用NULL
// If old_value is non-NULL, the buffer it points to is used to  return  the
// previous  value  of  the timer (i.e., the same information that is re‐
// turned by getitimer()).

// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);
// signum：要捕捉的信号
// handler：如何处理：SIG_IGN: 忽略; SIG_DFL 默认，相当于没有捕捉
//          如果是一个回调函数，程序员只负责写，是内核调用
// return: 返回上一次信号处理函数的地址，第一次是NULL，失败是SIG_ERR


// int main()
// {
//     pid_t pid = fork();

//     if(pid == 0)
//     {
//         int i=0;
//         for(int i=0; i<5; i++)
//         {
//             printf("child  process\n");
//             sleep(1);
//         }
//     }
//     else if(pid > 0)
//     {
//         printf("parent process\n");
//         sleep(3);
//         printf("kill now\n");
//         kill(pid, SIGINT);
//     }

//     return 0;
// }

// int main()
// {
//     int seconds = alarm(8);
//     printf("seconds = %d\n", seconds);

//     sleep(2);
//     seconds = alarm(1);  // 只计算cpu时间

//     printf("seconds : %d\n", seconds);

//     int i = 0;
//     while (1)
//     {
//         printf("%d\n", i++);
//     }
    
//     return 0;
// }


// typedef void (*sighandler_t)(int);
void myalarm(int num)
{
    printf("捕捉到的信号编号是：%d\n", num);
}

int main()
{
    // 注册好信号捕捉
    // signal(SIGALRM, SIG_IGN);
    // signal(SIGALRM, SIG_DFL;

   
    __sighandler_t res = signal(SIGALRM, myalarm);
    if(res == SIG_ERR)
    {
        perror("signal");
        exit(0);
    }

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

    getchar();

    return 0;


}
