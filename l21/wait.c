#include <sys/types.h>
#include <sys/wait.h>

// pid_t wait(int *wstatus);
// 等待一个子进程结束，如果任意一个子进程结束，它就会回收
// wstatus 一个int地址，传出参数
// return：成功：被回收的子进程id，失败：-1
// pid_t waitpid(pid_t pid, int *wstatus, int options);

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    for(int i=0; i<5; i++)
    {
        pid = fork();
        if(pid == 0)
            break;
    }

    if(pid > 0)
    {
        while(1)
        {
            printf("Parent, pid = %d\n", getpid());
            int st;
            // int ret = wait(NULL);  // 不想要获取状态
            int ret = wait(&st);  // 如果想要获取状态就传一个int的地址进去
            if(ret == -1)  // 没有子进程可以回收了会返回-1
                break;
            
            if(WIFEXITED(st))
                printf("退出的状态码： %d\n", WEXITSTATUS(st));  // 正常退出
            if(WIFSIGNALED(st))
                printf("干掉它的信号：%d\n", WTERMSIG(st));  // 被终止


            printf("Child die, pid = %d\n", ret);

            sleep(1);
        }
    }
    else if (pid == 0)
    {
        while(1)
        {
            printf("Child, pid = %d\n", getpid());
            // kill的时候搞快点，不然pid会变？不对啊应该不会变才对
            sleep(2);
        }
        exit(0);
    }

    return 0;
}