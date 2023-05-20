#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


// 3个窗口100张票，并发卖票
int tickets = 100;

void * sell(void * arg)
{
    while(tickets > 0)
    {
        usleep(3000);  // 休眠的时候另两个线程可能会进来，导致卖负数张票
        tickets--;
        printf("%ld窗口正在卖票, 剩余%d\n", pthread_self(), tickets);
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sell, NULL);
    pthread_create(&tid2, NULL, sell, NULL);
    pthread_create(&tid3, NULL, sell, NULL);

    // 用join就阻塞了，不如detach
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    pthread_exit(NULL);

    return 0;
}