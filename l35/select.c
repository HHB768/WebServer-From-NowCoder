#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    listen(lfd, 8);

    // 创建一个fd的集合
    fd_set rdset, tmp;  // 看源码，1024个文件描述符
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while(1)
    {
        tmp = rdset;

        // 调用select委托内核检查set
        int ret = select(maxfd+1, &tmp, NULL, NULL, NULL);  // timeout==NULL，永久阻塞到有数据来
        if(ret == -1)
        {
            perror("select");
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
            if(FD_ISSET(lfd, &tmp))
            {
                // 有新客户端来连接
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

                FD_SET(cfd, &rdset);

                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for(int fd = lfd + 1; fd <= maxfd; fd++)  // 总觉得不太好，真的lfd就是最小的吗
            // 也许可以从3开始，然后特判fd==lfd的时候是监听，其他是客户端
            {
                if(FD_ISSET(fd, &tmp))
                {
                    char buf[1024];
                    int len = read(fd, buf, sizeof(buf));
                    if(len == -1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if(len == 0)
                    {
                        printf("client close ... \n");
                        FD_CLR(fd, &rdset);
                    }
                    else if(len > 0)
                    {
                        printf("read buf = %s\n", buf);
                        write(fd, buf, strlen(buf) + 1);
                    }
                }
            }
        }

    }


    return 0;
}