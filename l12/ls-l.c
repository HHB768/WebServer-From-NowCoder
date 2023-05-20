// 模拟 ls -l 效果
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if(argc<2)
    {
        printf("ERROR, you should key in %s filename\n", argv[0]);
        return -1;
    }

    // agrv[0] = ./ls-l
    // agrv[1] = filename

    struct stat statbuf;

    int ret = lstat(argv[1], &statbuf);

    if(ret == -1)
    {
        perror("STAT ERROR");
        return -2;
    }

    char perms[11] = {0};  // 权限
    switch (statbuf.st_mode & __S_IFMT)
    {
    case __S_IFLNK:
        perms[0] = 'l';
        break;
    case __S_IFDIR:
        perms[0] = 'd';
        break;
    case __S_IFREG:
        perms[0] = '-';
        break;
    case __S_IFBLK:
        perms[0] = 'b';
        break;
    case __S_IFCHR:
        perms[0] = 'c';
        break;
    case __S_IFSOCK:
        perms[0] = 's';
        break;
    case __S_IFIFO:
        perms[0] = 'p';
        break;
    default:
        perms[0] = '?';
        break;
    }

    perms[1] = (statbuf.st_mode & S_IRUSR) ? 'r':'-';
    perms[2] = (statbuf.st_mode & S_IWUSR) ? 'w':'-';
    perms[3] = (statbuf.st_mode & S_IXUSR) ? 'x':'-';

    perms[4] = (statbuf.st_mode & S_IRGRP) ? 'r':'-';
    perms[5] = (statbuf.st_mode & S_IWGRP) ? 'w':'-';
    perms[6] = (statbuf.st_mode & S_IXGRP) ? 'x':'-';

    perms[7] = (statbuf.st_mode & S_IROTH) ? 'r':'-';
    perms[8] = (statbuf.st_mode & S_IWOTH) ? 'w':'-';
    perms[9] = (statbuf.st_mode & S_IXOTH) ? 'x':'-';

    // 点进去看类型
    // 用特定的函数提取出值
    // 不知道include哪个就去man里面找

    // statbuf.st_nlink;
    // getpwuid(statbuf.st_uid)->pw_name;
    // getgrgid(statbuf.st_gid)->gr_gid;
    // statbuf.st_size;  // long int
    // ctime(&statbuf.st_mtime);  
    // 将 long int 1980至今的秒数 转换成时间

    char buf[1024];

    // sprintf(buf, "%s %ld %s %s %ld %s %s", perms, statbuf.st_nlink, 
    // getpwuid(statbuf.st_uid)->pw_name, getgrgid(statbuf.st_gid)->gr_name, 
    // statbuf.st_size, ctime(&statbuf.st_mtime), argv[1]);
    // ctime输出的时间自带\n
    char mtime[512] = {0};
    char * time = ctime(&statbuf.st_mtime);
    strncpy(mtime, time, strlen(time)-1);

    sprintf(buf, "%s %ld %s %s %ld %s %s", perms, statbuf.st_nlink, 
    getpwuid(statbuf.st_uid)->pw_name, getgrgid(statbuf.st_gid)->gr_name, 
    statbuf.st_size, mtime, argv[1]);

    printf("%s\n", buf);
    return 0;
}