#define  _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    sigset_t set;

    sigemptyset(&set);

    int ret = sigismember(&set, SIGINT);
    if(ret == 0)
        printf("SIGINT 不阻塞\n");
    else
        printf("SIFINT 阻塞\n");

    sigaddset(&set, SIGINT);
    // sigaddset(&set, SIGQUIT);

    ret = sigismember(&set, SIGINT);
    if(ret == 0)
        printf("SIGINT 不阻塞\n");
    else
        printf("SIFINT 阻塞\n");
    
    ret = sigdelset(&set, SIGINT);
    ret = sigismember(&set, SIGINT);
    if(ret == 0)
        printf("SIGINT 不阻塞\n");
    else
        printf("SIFINT 阻塞\n");

    return 0;
}