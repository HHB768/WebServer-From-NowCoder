#include <pthread.h>

// pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t recmutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
// pthread_mutex_t errchkmutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
// int pthread_mutex_init(pthread_mutex_t  *mutex,  const  pthread_mutex‐
//                         attr_t *mutexattr);
// mutex:需要初始化的互斥量
// mutexattr：属性，一般NULL

// int pthread_mutex_lock(pthread_mutex_t *mutex);
// 加锁，其他线程在此阻塞

// int pthread_mutex_trylock(pthread_mutex_t *mutex);
// 尝试加锁，失败会直接返回

// int pthread_mutex_unlock(pthread_mutex_t *mutex);
// 解锁

// int pthread_mutex_destroy(pthread_mutex_t *mutex);
// 释放资源


#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


// 3个窗口100张票，并发卖票
int tickets = 100;
pthread_mutex_t mutex;

void * sell(void * arg)
{
    while(1)
    {
        // 其他业务...
        printf("%ld, dadada\n", pthread_self());

        // 加锁
        pthread_mutex_lock(&mutex);

        // 死锁
        // pthread_mutex_lock(&mutex);

        if(tickets > 0)
        {
            usleep(3000);  // 休眠的时候另两个线程可能会进来，导致卖负数张票
            tickets--;
            printf("%ld窗口正在卖票, 剩余%d\n", pthread_self(), tickets);
        }
        else
        {
            // 解锁
            pthread_mutex_unlock(&mutex);  // 不解也会死锁
            break;
        }

        // 解锁
            pthread_mutex_unlock(&mutex);  // 不解也会死锁
    }

    

    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sell, NULL);
    pthread_create(&tid2, NULL, sell, NULL);
    pthread_create(&tid3, NULL, sell, NULL);

    // 用join就阻塞着不要提前释放了
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}
