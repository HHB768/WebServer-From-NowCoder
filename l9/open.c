#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// c没有重载，这里是可变参数实现的

// 打开一个文件
// int open(const char *pathname, int flags);
// pathname: 文件路径
// flags: O_RDONLY, O_WRONLY, or O_RDWR (下面有)
// return value:
// open(), openat(), and creat() return the new file descriptor  (a  non‐
// negative integer), or -1 if an error occurred (in which case, errno is
// set appropriately).

// errno: linux系统函数库里面的全局变量，记录最近的错误号
#include <stdio.h>
// void perror(const char* s);
// 可以打印错误描述：s是用户自定义错误信息，输出的还有系统对错误的描述

// 创建一个文件（见create.c）
// int open(const char *pathname, int flags, mode_t mode);


// The  open()  system call opens the file specified by pathname.  If the
// specified file does not exist, it may optionally (if O_CREAT is speci‐
// fied in flags) be created by open().

// The  return value of open() is a file descriptor, a small, nonnegative
// integer that is used in subsequent system  calls  (read(2),  write(2),
// lseek(2),  fcntl(2),  etc.)  to  refer to the open file.  The file de‐
// scriptor returned by a successful call  will  be  the  lowest-numbered
// file descriptor not currently open for the process.

// By  default,  the  new file descriptor is set to remain open across an
// execve(2) (i.e., the FD_CLOEXEC file descriptor flag described in  fc‐
// ntl(2)  is  initially  disabled); the O_CLOEXEC flag, described below,
// can be used to change this default.  The file offset is set to the be‐
// ginning of the file (see lseek(2)).

// A  call to open() creates a new open file description, an entry in the
// system-wide table of open files.  The open  file  description  records
// the  file  offset  and  the file status flags (see below).  A file de‐
// scriptor is a reference to an open file description; this reference is
// unaffected if pathname is subsequently removed or modified to refer to
// a different file.  For further details on open file descriptions,  see
// NOTES.

// The  argument  flags  must  include one of the following access modes:
// O_RDONLY, O_WRONLY, or O_RDWR.  These request opening the  file  read-
// only, write-only, or read/write, respectively.

// In  addition,  zero  or more file creation flags and file status flags
// can be bitwise-or'd in flags.  The file creation flags are  O_CLOEXEC,
// O_CREAT,  O_DIRECTORY,  O_EXCL,  O_NOCTTY,  O_NOFOLLOW, O_TMPFILE, and
// O_TRUNC.  The file status flags are all of the remaining flags  listed
// below.   The distinction between these two groups of flags is that the
// file creation flags affect the semantics of the open operation itself,
// while the file status flags affect the semantics of subsequent I/O op‐
// erations.  The file status flags can be retrieved and (in some  cases)
// modified; see fcntl(2) for details.



#include <unistd.h>

int main()
{
    int fd = open("a.txt", O_RDONLY);
    if(fd == -1)
        perror("FILE OPEN ERROR");
    // printf -- FILE OPEN ERROR: No such file or directory

    close(fd);
}