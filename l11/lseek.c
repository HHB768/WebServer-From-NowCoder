#include <stdio.h>
// int fseek(FILE *stream, long offset, int whence);
// ERRORS
//     EINVAL The whence argument to fseek() was not SEEK_SET,  SEEK_END,  or
//                SEEK_CUR.  Or: the resulting file offset would be negative.

#include <sys/types.h>
#include <unistd.h>
// off_t lseek(int fd, off_t offset, int whence);
// offset: 偏移量
// whence: SEEK_SET：设置offset为偏移量
//         SEEK_CUR: 当前的文件指针+offset
//         SEEK_END: 文件大小+offset
// return: 返回最终所在的位置
// 1. lseek(fd, 0, SEEK_SET) 回到开头
// 2. lseek(fd, 0, SEEK_CUR) 不移动，返回当前位置
// 3. lseek(fd, 0, SEEK_END) 不移动，获取文件长度
// 4. lseek(fd, 100, SEEK_END) 增加文件长度100B
// 这也是大文件下载时的操作，先向系统占用到足够大小的空间
// 再慢慢向里面写满真实的数据

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = open("hello.txt", O_RDWR);
    if (fd == -1)
    {
        perror("OPEN ERROR");
        return -1;
    }

    int ret = lseek(fd, 100, SEEK_END);
    if(ret == -1)
    {
        perror("LSEEK ERROR");
        return -2;
    }
    
    const char* buff = " ";  // 不写这个的话中间的空字符不算文件大小
    int len = strlen(buff);
    write(fd, buff, len);

    close(fd);
    return 0;
    // 12 -> +100+1 = 113B
}