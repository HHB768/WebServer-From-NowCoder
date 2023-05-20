#include <sys/types.h>
#include <unistd.h>

// pid_t fork(void);
// 复制出一个一样的子进程，但占用不同的空间
// return: 返回两次，父进程中返回子进程ID or -1，子进程返回0或创建失败
// 失败：进程数上限/内存耗尽

// DESCRIPTION
// fork() creates a new process by duplicating the calling process.  The
// new process is referred to as the child process.  The calling process
// is referred to as the parent process.

// The  child process and the parent process run in separate memory spa‐
// ces.  At the time of fork() both memory spaces have the same content.
// Memory  writes,  file  mappings (mmap(2)), and unmappings (munmap(2))
// performed by one of the processes do not affect the other.

// RETURN VALUE
// On success, the PID of the child process is returned in  the  parent,
// and  0  is  returned in the child.  On failure, -1 is returned in the
// parent, no child process is created, and errno is set appropriately.

#include <stdio.h>

int main()
{
    pid_t pid = fork();

    if(pid > 0)
    {
        printf("pid: %d\n", pid);
        printf("I am a parent process, pid: %d, ppid: %d\n", getpid(), getppid());
    }
    else if(pid == 0)
    {
        sleep(2);
        printf("pid: %d\n", pid);
        printf("I am a child process, pid: %d, ppid: %d\n", getpid(), getppid());
    } // ppid 应该是 1

    for(int i=0; i<10; i++)
    {
        printf("i = %d, pid: %d\n", i, getpid());
        sleep(0.5);
    }
        
    return 0;
}

