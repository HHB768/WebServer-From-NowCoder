#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>


int main()
{
    unlink("server.sock");
    
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    bind(lfd, (struct sockaddr *)&addr, sizeof(addr));

    listen(lfd, 100);

    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    printf("client socket filename: %s\n", cliaddr.sun_path);

    while(1)
    {
        char buf[128];
        len = recv(cfd, buf, sizeof(buf), 0);

        if(len == -1)
        {
            perror("recv");
            exit(-1);
        }
        else if(len == 0)
        {
            printf("client closed ... \n");
            break;
        }
        else if(len > 0)
        {
            printf("client say : %s\n", buf);
            send(cfd, buf, len, 0);
        }
    }

    close(cfd);
    close(lfd);
    return 0;
}