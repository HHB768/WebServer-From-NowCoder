#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


// 3个窗口100张票，并发卖票
int tickets = 100;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void * sellA(void * arg)
{
    // 加锁
    pthread_mutex_lock(&mutex1);
    // 其他业务...
    sleep(1);
    pthread_mutex_lock(&mutex2);

    printf("WORKA...\n");
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void * sellB(void * arg)
{
    // 加锁
    pthread_mutex_lock(&mutex2);
    // 其他业务...
    sleep(1);
    pthread_mutex_lock(&mutex1);

    printf("WORKB...\n");
    
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}


int main()
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, sellA, NULL);
    pthread_create(&tid2, NULL, sellB, NULL);

    // 用join就阻塞着不要提前释放了
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    pthread_exit(NULL);

    return 0;
}
