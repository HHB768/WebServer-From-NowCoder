#include <pthread.h>

// int pthread_cancel(pthread_t thread);
// 取消某个线程，但不是立马终止，而是当它执行到一个取消点才终止
// 取消点可以粗略地理解为用户区到内核区的系统调用处

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg)
{
    printf("child thread: %ld\n", pthread_self());  // Printf也是一种取消点
    for(int i=0; i<10; i++)
    {
        printf("child : %d\n", i);  // Printf也是一种取消点
    }
    return NULL;
}

int main()
{
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0)
    {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());
    
    pthread_cancel(tid);

    pthread_exit(NULL);

    return 0;
}



