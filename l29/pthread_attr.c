// int pthread_attr_init(pthread_attr_t *attr);
// int pthread_attr_destroy(pthread_attr_t *attr);
// 创建和销毁线程属性
// int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
// int pthread_attr_getdetachstate(const pthread_attr_t *attr,
//                                 int *detachstate);
// detachstate：PTHREAD_CREATE_JOINABLE (default) PTHREAD_CREATE_DETACHED


#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg)
{
    printf("child thread: %ld\n", pthread_self());
    return NULL;
}
int main()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("size : %ld\n", size);

    pthread_t tid;

    int ret = pthread_create(&tid, &attr, callback, NULL);
    if(ret != 0)
    {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());
    
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;
}
