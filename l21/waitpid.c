// wait() and waitpid()
// The wait() system call suspends execution of the calling thread  until
// one of its children terminates.  The call wait(&wstatus) is equivalent
// to:

//     waitpid(-1, &wstatus, 0);

// The waitpid() system call suspends execution of the calling thread un‐
// til  a child specified by pid argument has changed state.  By default,
// waitpid() waits only for terminated children,  but  this  behavior  is
// modifiable via the options argument, as described below.

// The value of pid can be:

// < -1   meaning  wait  for  any child process whose process group ID is
//         equal to the absolute value of pid.

// -1     meaning wait for any child process.

// 0      meaning wait for any child process whose process  group  ID  is
//         equal to that of the calling process at the time of the call to
//         waitpid().

// > 0    meaning wait for the child whose process ID  is  equal  to  the
//         value of pid.

// The  value  of  options is an OR of zero or more of the following con‐
// stants:
 
// 注意：回收一个就跑
// pid > 0: 某个子进程的pid
// pid == 0: 回收当前进程组的所有子进程
// pid == -1: 回收所有，相当于wait
// pid < -1: 回收某个gid==abs(pid)的进程组的进程

// options: 0阻塞 WNOHANG不阻塞
// return value: >0 pid, 0 正常 -1错误或没有子进程


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
            sleep(1);
            // int ret = wait(NULL);  // 不想要获取状态
            int ret = waitpid(-1, &st, WNOHANG);  // 非阻塞，可以继续它自己的任务
            if(ret == -1)  // 没有子进程可以回收了会返回-1
                break;
            else if(ret == 0)
            {
                // 主进程的正常业务逻辑
                continue;
            }
            else if(ret > 0)
            {
                if(WIFEXITED(st))
                printf("退出的状态码： %d\n", WEXITSTATUS(st));  // 正常退出
                if(WIFSIGNALED(st))
                    printf("干掉它的信号：%d\n", WTERMSIG(st));  // 被终止


                printf("Child die, pid = %d\n", ret);
            }
            
            

            
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