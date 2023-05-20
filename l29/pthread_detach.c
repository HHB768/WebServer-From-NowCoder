// int pthread_detach(pthread_t thread);
// 设置分离则它运行结束后自动释放
// 不能多次detach也不能join一个detached的线程
// return：0 / errno

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
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0)
    {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());
    pthread_detach(tid);
    ret = pthread_join(tid, NULL);
    if(ret != 0)
    {
        char * errstr = strerror(ret);
        printf("error2 : %s\n", errstr);  // 运行会报错
    }

    sleep(3);

    return 0;
}

