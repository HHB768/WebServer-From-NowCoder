#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    // create listening socket
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);

    // binding a port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.186.129", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0 指所有网卡的ip都能绑定
    saddr.sin_port = htons(9999);
    bind(listenFd, (struct sockaddr *)&saddr, sizeof(saddr));

    // listen
    listen(listenFd, 8);

    // accept
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientFd = accept(listenFd, (struct sockaddr *)&clientaddr, &len);

    if(clientFd == -1)
    {
        perror("client fd error");
        exit(-1);
    }

    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);

    printf("client ip : %s, port : %d\n", clientIP, clientPort);

    // recv
    char recvBuf[1024] = {0};
    int recvLen = read(clientFd, recvBuf, sizeof(recvBuf));

    if(recvLen == 0)
    {
        printf("client offline...\n");
    }
    else if(recvLen > 0)
    {
        printf("receive from client: %s\n", recvBuf);
    }

    // send
    char * data = "hello, i am server\n";
    write(clientFd, data, strlen(data));


    close(clientFd);
    close(listenFd);

    return 0;
}