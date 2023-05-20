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
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>


int main()
{
    int fd = open("english.txt", O_RDONLY);
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    int len = lseek(fd, 0, SEEK_END);

    truncate("cpy.txt", len);
    write(fd1, " ", 1);  // 这里是没必要的，因为truncate改变了大小并不移动指针
    // 这里写在了文件开头，不影响大小，若是lseek+write则文件大小为len+1

    void * ptr = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
    void * ptr1 = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd1, 0);
    // 相当于把文件内容全部映射到内存，交由这个指针控制

    if(ptr == MAP_FAILED || ptr1 == MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }

    memcpy(ptr1, ptr, len);  // 简单有效的拷贝！

    munmap(ptr1, len);
    munmap(ptr, len);
    close(fd1);
    close(fd);
    
    return 0;
}