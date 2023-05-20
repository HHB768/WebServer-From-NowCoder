#include <sys/mman.h>

// void *mmap(void *addr, size_t length, int prot, int flags,
//                   int fd, off_t offset);
// 将文件数据映射到内存中（文件映射段）

// addr：一般是NULL，即交给内核指定
// length：映射的数据长度，一般是文件的长度  stat/lseek  // 系统会帮你补全到整数个页

// prot(ection):  可以bitwise操作  PROT_READ | PROT_WRITE
//              PROT_EXEC  Pages may be executed.
//              PROT_READ  Pages may be read. // 你一般要有这个不然什么都做不了...
//              PROT_WRITE Pages may be written.
//              PROT_NONE  Pages may not be accessed.
// 不可与 open 权限冲突，不能扩权，其他按需

// flag：MAP_SHARED: 映射区数据会和磁盘文件同步，进程间通信用这个
//       MAP_PRIVATE：不同步，如果数据改变了，会拷贝一个副本
// fd： open得到的fd，文件大小不能为0
// offset：必须是4k的整数倍才能成功，一般不用

//return：返回创建的内存的首地址，失败返回MAP_FAILED ((void*)-1)


// int munmap(void *addr, size_t length);
// 释放内存映射
// addr就是mmap的返回值，length就是前面申请的长度


// fork之前创建内存映射区，两个进程可以共享内存映射区，和pipe的逻辑差不多
// 没有亲缘关系的两个进程，分别得到操作某文件的指针然后通信，注意文件大小不为0
// 内存映射区不阻塞的，和 pipe / fifo 不同

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>


int main()
{
    int fd = open("test.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);

    void * ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("MMAP");
        exit(0);
    }

    pid_t pid = fork();

    if(pid>0)
    {
        wait(NULL);
        char buf[64];
        strcpy(buf, (char *)ptr);
        // 文件的内容其实全部进入了内存，但是字符串遇到'\0'就不看后面了
        printf("read data: %s\n", buf);
    }
    else if (pid==0)
    {
        strcpy((char *)ptr, "nihaoa!");
    }

    munmap(ptr, size);

    return 0;
}