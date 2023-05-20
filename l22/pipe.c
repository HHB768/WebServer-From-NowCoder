#include <unistd.h>

// /* On Alpha, IA-64, MIPS, SuperH, and SPARC/SPARC64; see NOTES */
// struct fd_pair {
//     long fd[2];
// };
// struct fd_pair pipe();

// /* On all other architectures */
// int pipe(int pipefd[2]);

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>

// int pipe2(int pipefd[2], int flags);

// 数组pipefd是传出参数，[0]是读端[1]是写端
// 注意，只能亲缘关系的进程间通信

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int pipefd[2];
    int ret = pipe(pipefd);

    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("pip size : %ld\n", size);

    if(ret == -1)
    {
        perror("pipe");
        exit(0);
    }
    pid_t pid = fork();
    if(pid > 0)
    {
        printf("i am a parent, pid : %d\n", getpid());
        char buf[1024] = {0}; 
        char* str = "hello, i am a parent";
        while(1)
        {
            int len = read(pipefd[0], buf, sizeof(buf));  // 如果管道里没有数据，read会阻塞
            printf("parent rev : %s, pid : %d\n", buf, getpid());

            write(pipefd[1], str, strlen(str));
            sleep(2);  // 注意write后面加sleep，不然自己的read给读出来了  // 但是不能完全解决
            // 可以在输出端观察到 childt
            // 所以在实际使用中一般一个方向传数据
        }
    }    
    else if(pid == 0)
    {
        printf("i am a child, pid : %d\n", getpid());
        char* str = "hello, i am a child";
        char buf[1024] = {0}; 
        while(1)
        {
            
            write(pipefd[1], str, strlen(str));  // 如果管道满了，write阻塞
            sleep(2);
            int len = read(pipefd[0], buf, sizeof(buf)); 
            printf("child rev : %s, pid : %d\n", buf, getpid());
        }
        
    }

    return 0;
}
