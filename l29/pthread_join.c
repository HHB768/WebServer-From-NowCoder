// int pthread_join(pthread_t thread, void **retval);
// 和一个已经终止的线程进行连接
// 回收子线程资源，阻塞函数，一次回收一个
// 可以在子线程中调用，但是一般还是在主线程
// thread：要回收的线程
// retval: 接受子线程退出时返回值
// (二级指针，需要把一个指针的地址传过来，join会把它定位到合适的返回值的地址上)

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int value = 10;
void * callback(void * arg)
{
    printf("child thread id : %ld\n", pthread_self());
    sleep(3);
    // return NULL;
    pthread_exit((void *)&value);  // == return (void *)&value;
}
int main()
{
    printf("parent thread id : %ld\n", pthread_self());

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0)
    {
        char * str = strerror(ret);
        printf("error : %s\n", str);
    }

    int * thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);  // 阻塞
    if(ret != 0)
    {
        char * str = strerror(ret);
        printf("error : %s\n", str);
    }
    printf("回收成功, exit value: %d\n", *thread_retval);
    pthread_exit(NULL);  // 这样退出不会影响子线程

    return 0;  // 这里没了子线程就可能没执行完
}


