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
#include <wait.h>


int main()
{
    int fd[2];
    int ret = pipe(fd);

    if(ret == -1)
    {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if(pid > 0)
    {
        // 关闭写端
        close(fd[1]);
        
        char buf[1024] = {0};
        int len = -1;
        while((len = read(fd[0], buf, sizeof(buf) - 1)) > 0)
        {
            printf("%s", buf);
            memset(buf, 0, 1024);
        }
        wait(NULL);
    }
    else if(pid == 0)
    {
        // 关闭读端
        close(fd[0]);

        dup2(fd[1], STDOUT_FILENO);
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);
    }
    else
    {
        perror("fork");
        exit(0);
    }
}