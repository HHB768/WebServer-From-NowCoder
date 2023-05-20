#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// int stat(const char *pathname, struct stat *statbuf);
// statbuf: 传出参数，保存获取的信息
// return:  成功0，失败-1
// int fstat(int fd, struct stat *statbuf);
// int lstat(const char *pathname, struct stat *statbuf);

// These functions return information about a file, in the buffer pointed
// to by statbuf.  No permissions are required on the file itself, but—in
// the case of stat(), fstatat(), and lstat()—execute (search) permission
// is required on all of the directories in pathname  that  lead  to  the
// file.

// stat() and fstatat() retrieve information about the file pointed to by
// pathname; the differences for fstatat() are described below.

// lstat() is identical to stat(), except that if pathname is a  symbolic
// link,  then it returns information about the link itself, not the file
// that the link refers to.
// These functions return information about a file, in the buffer pointed
// to by statbuf.  No permissions are required on the file itself, but—in
// the case of stat(), fstatat(), and lstat()—execute (search) permission
// is required on all of the directories in pathname  that  lead  to  the
// file.

// stat() and fstatat() retrieve information about the file pointed to by
// pathname; the differences for fstatat() are described below.

// lstat() is identical to stat(), except that if pathname is a  symbolic
// link,  then it returns information about the link itself, not the file
// that the link refers to.

#include <stdio.h>

int main()
{
    struct stat statbuf;
    // int ret = stat("stat.c", &statbuf);
    int ret = lstat("stat1.c", &statbuf);

    if(ret == -1)
    {
        perror("STAT ERROR");
        return -1;
    }

    printf("size: %ld\n", statbuf.st_size);
    return 0;
}

