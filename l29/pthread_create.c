#include <pthread.h>

// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//                     void *(*start_routine) (void *), void *arg);
// Compile and link with -pthread.
// thread： 传出参数，子线程的线程id
// attr：线程属性，不想设置就NULL
// start_routine: 函数指针，子线程需要处理的逻辑代码
// arg：给第三个函数传参
// return: 成功0，失败返回错误号，用 strerror(int errno) 处理

#include <stdio.h>
#include <string.h>
#include <unistd.h>


void * callback(void * arg)
{
    printf("threads ...\n");
    printf("arg value : %d\n", *(int *)arg);
    return NULL;
}
int main()
{
    pthread_t tid;
    int num = 10;

    int ret = pthread_create(&tid, NULL, callback, (void *)&num);
    if(ret != 0)
    {
        char * errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d\n", i);
    }
    sleep(5);
    return 0;
}