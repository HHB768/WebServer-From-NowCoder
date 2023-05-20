#include <sys/ipc.h>
#include <sys/shm.h>

// int shmget(key_t key, size_t size, int shmflg);
// allocates a System V shared memory segment

// 功能：创建一个新的内存共享段，或者获取一个既有的共享内存段的标识
// 新创建的内存段中的数据都初始化为0
// key: 找到或创建一个共享内存（自动转换为非零16进制）
// size: 共享内存大小，补全到1个页
// shflg: 
//        IPC_CREAT Create  a new segment.  创建
//        IPC_EXCL  判断是否存在
//           This  flag is used with IPC_CREAT to ensure that this call cre‐
//           ates the segment.  If the  segment  already  exists,  the  call
//           fails.
// return: >0 共享内存的id，-1失败

// A  new  shared  memory  segment,  with size equal to the value of size
//        rounded up to a multiple of PAGE_SIZE, is created if key has the value
//        IPC_PRIVATE  or key isn't IPC_PRIVATE, no shared memory segment corre‐
//        sponding to key exists, and IPC_CREAT is specified in shmflg.


// void *shmat(int shmid, const void *shmaddr, int shmflg);
// shmid: shmget返回的id
// shmaddr：在内存中的地址，一般用NULL
// shmflg： 读权限：SHM_RDONLY，读写权限：0   If this flag  is  not  speci‐
            //   fied,  the  segment  is attached for read and write access
// return：成功返回内存的首地址，失败 (void *)-1

// int shmdt(const void *shmaddr);  // detach

// int shmctl(int shmid, int cmd, struct shmid_ds *buf);
// 对共享内存的操作，特别是删除
// cmd: 要做的操作：IPC_STAT:获取buf状态  IPC_SET:设置buf状态   
//                 IPC_RMID:标记删除，即所有引用解除后删除
// buf：要设置的属性

// key_t ftok(const char *pathname, int proj_id);
// 根据路径和一个int8，生成一个共享内存key，可以自动化地为你的shmget的第一个参数选择key
// 不同的path或projid会有不同的结果

#include <string.h>
#include <stdio.h>

int main()
{
    int shmid = shmget(100, 4096, IPC_CREAT|0664);
    printf("%d", shmid);
    void * ptr = shmat(shmid, NULL, 0);
    char * str = "helloworld";
    memcpy(ptr, str, strlen(str)+1);
    
    getchar();
    
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
