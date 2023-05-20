#define _GNU_SOURCE
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recycleChild(int arg)
{
    while(1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1)
        {
            printf("没有子进程需要回收了\n");
            break;
        }
        else if(ret == 0)
        {
            printf("还有子进程活着\n");
            break;
        }
        else
        {
            printf("子进程 %d 被回收了\n", ret);
        }
    }
}

int main()
{
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;

    sigaction(SIGCHLD, &act, NULL);

    // create listening socket
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    // if fail ...

    // binding a port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    bind(listenFd, (struct sockaddr *)&saddr, sizeof(saddr));

    // listen
    listen(listenFd, 128);

    // 主进程循环accept
    while(1)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int clientFd = accept(listenFd, (struct sockaddr *)&clientaddr, &len);
        // 如果在此阻塞时被回调函数打断，会报错
        // EINTR  The system call was interrupted by a signal that was caught be‐
        //        fore a valid connection arrived

        if(clientFd == -1)
        {
            if(errno == EINTR)
                continue;
            perror("client fd error");
            exit(-1);
        }

        // 新建一个子进程专门用来为这个客户通信
        pid_t  pid = fork();
        if(pid == 0)
        {
            char clientIP[16];
            inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
            unsigned short clientPort = ntohs(clientaddr.sin_port);

            printf("client ip : %s, port : %d\n", clientIP, clientPort);
            
            // recv
            char recvBuf[1024] = {0};
            while(1)
            {
                
                int recvLen = read(clientFd, recvBuf, sizeof(recvBuf));
                // 可能遇到 connection reset by peer
                if(recvLen == 0)
                {
                    printf("client offline...\n");
                    break;
                }
                else if(recvLen > 0)
                {
                    printf("receive from client: %s\n", recvBuf);
                }
                else
                {
                    printf("client error...\n");
                    break;
                }

                // send
                // 回射服务器
                write(clientFd, recvBuf, strlen(recvBuf)+1);
                // 如果在这里客户端刚好退出了，直接 SIGPIPE
            }
            close(clientFd);
            exit(0);  // 退出子进程
        }
    }
    
    close(listenFd);

    return 0;
}