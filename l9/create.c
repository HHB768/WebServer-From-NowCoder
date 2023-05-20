#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// int open(const char *pathname, int flags, mode_t mode);
// flags: 除了那三个必选项还有可选项：O_APPEND, O_CREAT, O_ASYNC, O_CLOEXEC
// O_CREAT: If pathname does not exist, create it as a regular file.
// mode: 8进制数，表明操作权限：如0775

// The  mode  argument  specifies the file mode bits to be applied
// when a new file is created.  If neither O_CREAT  nor  O_TMPFILE
// is  specified  in  flags, then mode is ignored (and can thus be
// specified as 0, or simply omitted)
// 最终权限是 mode & ~umask
// 0是八进制，775三位分别是用户权限、组权限、其他组用户权限，每个数字的三个bit
// 表明rwx的权限 mode:rwx=0777, 用户默认（可以 umask val 修改）
// 例如umask=0002 ~umask=0775 最终 111111111 & 111111101 = 0775

int main()
{
    int fd = open("create.txt", O_RDWR  | O_CREAT, 0777);
    // 使用 | 来合并指令
    if(fd == -1)
        perror("OPEN ERROR");
    
    // 最终 mode rwxrwxr-x  // ll看，第一位是文件（夹）类型
    close(fd);
    return 0;
}