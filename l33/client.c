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
    inet_pton(AF_INET, "192.168.186.129", &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999);
    connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));

    char recvBuf[1024] = {0};

    for(int i=0; i<50; i++)
    {
        sprintf(recvBuf, "data: %d\n", i);
        write(fd, recvBuf, strlen(recvBuf)+1);

        sleep(1);
        
        int recvLen = read(fd, recvBuf, sizeof(recvBuf));

        if(recvLen == 0)
        {
            printf("server offline...\n");
            break;
        }
        else if(recvLen > 0)
        {
            printf("receive from server: %s\n", recvBuf);
        }
        else
        {
            perror("read");
            exit(-1);
        }
    }
    
    close(fd);

    return 0;
}