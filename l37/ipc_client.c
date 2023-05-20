#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>


int main()
{
    unlink("client.sock");

    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "client.sock");
    bind(cfd, (struct sockaddr *)&addr, sizeof(addr));

    struct sockaddr_un seraddr;
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path, "server.sock");
    connect(cfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    int num = 0;
    while(1)
    {
        char buf[128];
        sprintf(buf, "hello, i am client %d\n", num++);
        int len = send(cfd, buf, strlen(buf)+1, 0);

        len = recv(cfd, buf, sizeof(buf), 0);

        if(len == -1)
        {
            perror("recv");
            exit(-1);
        }
        else if(len == 0)
        {
            printf("server closed ... \n");
            break;
        }
        else if(len > 0)
        {
            printf("server say : %s\n", buf);
            send(cfd, buf, len, 0);
        }

        sleep(1);
    }

    close(cfd);
    return 0;
}