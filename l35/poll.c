#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>


#define MAX_CLIENT_COUNT 1024

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    listen(lfd, 8);

    // 初始化文件描述符数组
    struct pollfd fds[MAX_CLIENT_COUNT];
    for(int i=0; i<MAX_CLIENT_COUNT; i++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    nfds_t nfds = 0;

    while(1)
    {
        // 调用poll委托内核检查set
        int ret = poll(fds, nfds + 1, -1);
          // timeout==-1，永久阻塞到有数据来
        if(ret == -1)
        {
            perror("poll");
            exit(-1);
        }
        else if(ret == 0)
        {
            // 超时不返回
            // 但是我们没有哒
            printf("dadada\n");
            continue;
        }
        else if(ret > 0)
        {
            if(fds[0].revents & POLLIN)
            {
                // 有新客户端来连接
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

                for(int i=0+1; i<MAX_CLIENT_COUNT; i++)
                {
                    if(fds[i].fd == -1)
                    {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                nfds = nfds > cfd ? nfds : cfd;
            }

            for(int i = 1; i <= nfds; i++)
            {
                if(fds[i].revents & POLLIN)
                {
                    char buf[1024];
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if(len == -1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if(len == 0)
                    {
                        printf("client close ... \n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }
                    else if(len > 0)
                    {
                        printf("read buf = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }
        }

    }


    return 0;
}