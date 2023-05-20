#include <unistd.h>
// int access(const char *pathname, int mode);
// mode: R_OK W_OK X_OK F_OK(是否存在该文件)
// return ：0, 1
#include <sys/stat.h>
// int chmod(const char *pathname, mode_t mode);
// mode: 八进制数/宏值
// int chown(const char *pathname, uid_t owner, gid_t group);
#include <sys/types.h>
// int truncate(const char *path, off_t length);
// 缩减/拓展文件至指定大小(length)

#include <stdio.h>

int main()
{
    // int ret = access("a.txt", F_OK);
    // int ret = chmod("a.txt", 0777);
    int ret = truncate("a.txt", 5);
    // int ret = truncate("a.txt", 20);
    printf("%d\n", ret);
    return 0;
}
