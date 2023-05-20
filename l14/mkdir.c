#include <sys/stat.h>
#include <sys/types.h>

// int mkdir(const char *pathname, mode_t mode);
// int rmdir(const char *pathname)
// mkdir() attempts to create a directory named pathname.

// The  argument  mode  specifies the mode for the new directory (see in‐
// ode(7)).  It is modified by the process's umask in the usual  way:  in
// the  absence  of  a  default ACL, the mode of the created directory is
// (mode & ~umask & 0777).  Whether other mode bits are honored  for  the
// created  directory  depends  on  the operating system.  For Linux, see
// NOTES below.

#include <stdio.h>
// int rename(const char *oldpath, const char *newpath);
// 修改进程的工作目录
// int chdir(const char *path);
// 获取目录
// char *getcwd(char *buf, size_t size);
// 返回的就是buf，也就是路径字符串指针
// char *getwd(char *buf);
// char *get_current_dir_name(void);
#include <unistd.h>
#include <fcntl.h>

int main()
{
    mkdir("aaa", 0777);
    rename("aaa", "bbb");
    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("当前的目录是： %s\n", buf);
    int ret = chdir("/home/nowcoder/linux/l13");
    if(ret == -1)
        perror("CHDIR");
    
    open("chdir.txt", O_CREAT | O_RDWR, 0664);
    getcwd(buf, sizeof(buf));
    printf("当前的目录是： %s\n", buf);

    return 0;
}