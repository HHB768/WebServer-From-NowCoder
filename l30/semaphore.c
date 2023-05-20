#include <semaphore.h>
// int sem_init(sem_t *sem, int pshared, unsigned int value);
// pshare: 0 线程间通信，非 0 进程间

// int sem_wait(sem_t *sem);
// 对 value --，若是到 0 则阻塞，直到非 0
// int sem_trywait(sem_t *sem);
// int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

// int sem_destroy(sem_t *sem);
// 释放
// int sem_post(sem_t *sem);
// 解锁，value ++
// int sem_getvalue(sem_t *sem, int *sval);

/*
    sem_t psem;
    sem_t csem;
    init(psem, 0, 8);
    init(csem, 0, 0);

    preoducer()
    {
        sem_wait(&psem);  // 生产一次
        sem_post(&cesm);  // 提醒消费者可以消费
    }

    customer()
    {
        sem_wait(&csem);  // 消费一次
        sem_post(&psem);  // 提醒生产者生产
    }

*/

#define _GNU_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct Node{
    int num;
    struct Node* next;
};

struct Node* head = NULL;
pthread_mutex_t mutex;
sem_t psem, csem;

void * produce(void * arg)
{
    while(1)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
        // 头插法
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add num: %d, tid: %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        usleep(1000);
    }
    return NULL;
}

void * custom(void * arg)
{
    while(1)
    {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node * tmp = head;

        head = head->next;
        printf("delete num: %d, tid: %ld\n", tmp->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(1000);
    }
    return NULL;
}


int main()
{
    // 5 生产者，5 消费者
    pthread_t ptids[5];
    pthread_t ctids[5];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);


    for(int i=0; i<5; i++)
    {
        pthread_create(&ptids[i], NULL, produce, NULL);
        pthread_create(&ctids[i], NULL, custom, NULL);
    }

    for(int i=0; i<5; i++)
    {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    pthread_exit(NULL);

    // destroy

    return 0;
}
