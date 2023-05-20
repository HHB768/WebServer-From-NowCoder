#include <sys/types.h>
#include <sys/stat.h>

// int mkfifo(const char *pathname, mode_t mode);
// mode 权限，和 open 时创建是一样的
// return: 0 / -1
// mkfifo() makes a FIFO special file with name pathname.  mode specifies
// the FIFO's permissions.  It is modified by the process's umask in  the
// usual way: the permissions of the created file are (mode & ~umask).

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    int fd = open("fifo1", O_RDONLY);

    while (1)
    {
        // sleep(2);
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == 0)
        {
            printf("写端无了...\n");
            break;
        }
        printf("recv buf : %s\n", buf);
    }
    close(fd);

    return 0;
}