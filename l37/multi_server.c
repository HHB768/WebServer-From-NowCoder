#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {

    // 1.创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    
    if(fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.设置多播属性
    struct in_addr imr_multiaddr;
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    // 3.初始化客户端地址
    struct sockaddr_in cliaddr;
    // struct sockaddr_in addr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "239.0.0.10", &cliaddr.sin_addr.s_addr);

    // 不用绑定服务端自己的addr了，因为不需要监听客户端的数据，只需要发

    // 4.通信
    int num = 0;
    while(1) {
        char sendBuf[128];
        sprintf(sendBuf, "hello client ... %d\n", num++);

        // 发送数据
        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        printf("组播数据： %s\n", sendBuf);
        sleep(1);

    }

    close(fd);
    return 0;
}