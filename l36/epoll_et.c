#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>


int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    listen(lfd, 8);

    int epfd = epoll_create(42);
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1)
    {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1)
        {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++)
        {
            int curfd = epevs[i].data.fd;
            if(curfd == lfd)
            {
                // 新客户端到达
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                epev.events = EPOLLIN | EPOLLET;  // 边缘触发
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }
            else if(epevs[i].events & EPOLLIN)
            {
                // 通信
                char buf[5];  // 只准读5个字节 >_<
                // 循环读取所有数据
                
                int len = 0;
                while((len = read(curfd, buf, sizeof(buf)))>0)
                {
                    printf("recv: %s\n", buf);
                    write(curfd, buf, len);
                }
                if(len == 0)
                {
                    printf("client close ... \n");
                }
                else if(len == -1)
                {
                    if(errno == EAGAIN)
                    {
                        printf("data over ... \n");
                        // 数据读完了 // 阻塞读完了会卡在这继续等待read
                        // 只有非阻塞要特判
                    }
                    else
                    {
                        perror("read");  // 错误了
                        exit(-1);
                    }
                    
                }
            }
        }
    }
    close(lfd);
    close(epfd);


    return 0;
}