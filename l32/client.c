#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    // create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // connect
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.186.129", saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999);
    connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));

    // send
    char * data = "hello, i am client\n";
    write(fd, data, strlen(data));

    // recv
    char recvBuf[1024] = {0};
    int recvLen = read(fd, recvBuf, sizeof(recvBuf));

    if(recvLen == 0)
    {
        printf("server offline...\n");
    }
    else if(recvLen > 0)
    {
        printf("receive from server: %s\n", recvBuf);
    }

    close(fd);

    return 0;
}