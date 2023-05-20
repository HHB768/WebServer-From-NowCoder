// void pthread_exit(void *retval);
// retval：传递一个指针作为返回，在join里面有用

#include <pthread.h>
#include <stdio.h>
#include <string.h>


void * callback(void * arg)
{
    printf("child thread id : %ld\n", pthread_self());
    return NULL;
}
int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    printf("parent thread id : %ld\n", pthread_self());

    if(ret != 0)
    {
        char * str = strerror(ret);
        printf("error : %s\n", str);
    }

    pthread_exit(NULL);  // 这样退出不会影响子线程

    return 0;  // 这里没了子线程就可能没执行完
}