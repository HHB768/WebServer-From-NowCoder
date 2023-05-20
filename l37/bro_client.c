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

    // 2.客户端绑定接受广播的端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    int num = 0;
    // 3.通信
    while(1) {
        char buf[128];
        // 接收数据
        int num = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);  // 不用指定目标服务器嘞
        printf("server say : %s\n", buf);

        sleep(1);
    }

    close(fd);
    return 0;
}