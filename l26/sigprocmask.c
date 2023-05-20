// examine and change blocked signals:
// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
// how: SIG_BLOCK: 将新的阻塞信号集添加进去：oriset |= set
//      SIG_UNBLOCK: 将值为阻塞信号从系统中扣除：oriset &= ~set
//      SIG_SETMASK: 覆盖为新信号集：oriset = set
// set: 自定信号集
// oldset: 保存之前的内核中的阻塞信号集的状态，一般不用，为NULL
// return: 0 / -1 错误号：EFAULT、EINVAL

// int sigpending(sigset_t *set);
// 获取未决信号集并存在set中

#define  _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    // 通过阻塞某些信号，然后通过键盘产生它们
    // 然后查看未决信号集
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    sigprocmask(SIG_BLOCK, &set, NULL);

    int num = 0;
    while (1)
    {
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        for(int i=1; i<=32; i++)
        {
            if(sigismember(&pendingset, i)==1)
                printf("1");
            else if(sigismember(&pendingset, i)==0)
                printf("0");
            else
            {
                perror("pendingset");
                exit(0);
            }

            
        }
        printf("\n");
        sleep(1);

        num++;
        if(num == 10)
        {
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }
    
    return 0;
}