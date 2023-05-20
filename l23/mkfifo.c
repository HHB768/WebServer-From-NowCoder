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


int main()
{
    if(access("fifo1", F_OK) != 0)
    {
        int ret = mkfifo("fifo1", 0664);
        if(ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }
    

    return 0;
}