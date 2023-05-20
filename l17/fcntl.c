#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// int fcntl(int fd, int cmd, ... /* arg */ );
// cmd: F_DUPFD: return newfd = dup(oldfd)
//      F_GETFL: 获取文件状态flag，就是open那个flag
//      F_SETFL: 设置flag: access mode: O_RDONLY, O_WRONLY, O_RDWR
//                            optioanl: O_APPEND(追加), O_NONBLOCK(非阻塞调用)


// DESCRIPTION
// fcntl()  performs  one  of  the operations described below on the open
// file descriptor fd.  The operation is determined by cmd.

// fcntl() can take an optional third argument.  Whether or not this  ar‐
// gument  is  required is determined by cmd.  The required argument type
// is indicated in parentheses after each cmd name (in  most  cases,  the
// required  type  is  int,  and  we identify the argument using the name
// arg), or void is specified if the argument is not required.

// Certain of the operations below are supported only since a  particular
// Linux  kernel  version.   The preferred method of checking whether the
// host kernel supports a particular operation is to invoke fcntl()  with
// the  desired cmd value and then test whether the call failed with EIN‐
// VAL, indicating that the kernel does not recognize this value.

int main()
{
    int fd = open("1.txt", O_RDONLY);
    int ret = fcntl(fd, F_DUPFD);  // 复制

    int flag = fcntl(fd, F_GETFL);  // 读取设置
    fcntl(fd, F_SETFL, O_APPEND | O_RDWR);  // 追加设置 
    // 但是 access mode 以及一些别的 mode 是无法用 fcntl 追加的
    // 所以这里什么都不会发生
    char* str = "nihao";
    write(fd, str, strlen(str));

    int fd2 = open("1.txt", O_RDWR);
    fcntl(fd2, F_SETFL, flag | O_APPEND);  // 追加设置
    write(fd2, str, strlen(str));
    
    return 0;
}