#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


int main()
{
    if(access("fifo1", F_OK)!=0)
    {
        printf("管道不存在,创建FIFO1\n");
        int ret = mkfifo("fifo1", 0664);
        if(ret == -1)
        {
            perror("mkpipo1");
            exit(0);
        }
    }

    if(access("fifo2", F_OK)!=0)
    {
        printf("管道不存在,创建FIFO2\n");
        int ret = mkfifo("fifo2", 0664);
        if(ret == -1)
        {
            perror("mkpipo2");
            exit(0);
        }
    }

    int fdr = open("fifo1", O_RDONLY);
    if(fdr == -1)
    {
        perror("open read fifo");
        exit(0);
    }
    printf("打开fifo1, 等待读入数据...\n");
    int fdw = open("fifo2", O_WRONLY);
    if(fdw == -1)
    {
        perror("open write fifo");
        exit(0);
    }
    printf("打开fifo2, 等待写入数据...\n");

    char buf[128];
    while(1)
    {
        memset(buf, 0, 128);
        
        int len = read(fdr, buf, 128);
        if(len <= 0)
        {
            // 对方关闭或是错误
            break;
        }
        printf("buf : %s\n", buf);
        
        memset(buf, 0, 128);
        fgets(buf, 128, stdin);  // STDOUT_FILENO 是一个fd，值为1
        // 从键盘获取输入到缓冲区
        int ret = write(fdw, buf, strlen(buf));
        if(ret == -1)
        {
            perror("write");
            exit(0);
        }

        
    }
    close(fdr);
    close(fdw);

    return 0;
}