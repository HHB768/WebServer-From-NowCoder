#define _GNU_SOURCE
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


struct sockInfo{
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};

struct sockInfo sockInfos[128];

void * working(void * arg)
{
    struct sockInfo * pinfo = (struct sockInfo *)arg;
    char clientIP[16];
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(pinfo->addr.sin_port);

    printf("client ip : %s, port : %d\n", clientIP, clientPort);
    
    // recv
    char recvBuf[1024] = {0};
    while(1)
    {
        
        int recvLen = read(pinfo->fd, recvBuf, sizeof(recvBuf));
        // 可能遇到 connection reset by peer
        if(recvLen == 0)
        {
            printf("client offline...\n");
            break;
        }
        else if(recvLen > 0)
        {
            printf("receive from client: %s\n", recvBuf);
            printf("fd num: %d\n", pinfo->fd);
        }
        else
        {
            printf("client error...\n");
            break;
        }

        // send
        // 回射服务器
        write(pinfo->fd, recvBuf, strlen(recvBuf)+1);
        // 如果在这里客户端刚好退出了，直接 SIGPIPE
    }
    close(pinfo->fd);
    pinfo->fd = -1;

    return NULL;
}

int main()
{
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

    int max = sizeof(sockInfos) / sizeof(sockInfos[0]);
    for(int i=0; i<max; i++)
    {
        bzero(&sockInfos[i], sizeof(sockInfos[i]));
        sockInfos[i].fd = -1;
        sockInfos[i].tid = -1;
    }

    // 主进程循环accept
    while(1)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int clientFd = accept(listenFd, (struct sockaddr *)&clientaddr, &len);

        struct sockInfo * pinfo;
        for(int i=0; i<max; i++)
        {
            if(sockInfos[i].fd==-1)
            {
                pinfo = &sockInfos[i];
                break;
            }
            if(i==max-1)
            {
                sleep(1);
                i = 0;  // 回到第一个去找，看看有没有空闲下来的
            }
        }
        pinfo->fd = clientFd;
        memcpy(&pinfo->addr, &clientaddr, len);

        // 新建一个子线程专门用来为这个客户通信
        pthread_t tid;
        pthread_create(&pinfo->tid, NULL, working, pinfo);

        pthread_detach(pinfo->tid);
    }
    
    close(listenFd);

    return 0;
}