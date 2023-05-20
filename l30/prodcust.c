#define _GNU_SOURCE 1
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// int pthread_cond_init(pthread_cond_t    *cond,   pthread_condattr_t
//                     *cond_attr);
// int pthread_cond_signal(pthread_cond_t *cond);
// 唤醒线程
// int pthread_cond_broadcast(pthread_cond_t *cond);
// 唤醒所有线程
// int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
// 阻塞函数，线程会等待
// int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t  *mu‐
//                     tex, const struct timespec *abstime);
// 等待一定的时间 (abstime)
// int pthread_cond_destroy(pthread_cond_t *cond);


struct Node{
    int num;
    struct Node* next;
};

struct Node* head = NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;

void * produce(void * arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
        // 头插法
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add num: %d, tid: %ld\n", newNode->num, pthread_self());
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
    return NULL;
}

void * custom(void * arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        struct Node * tmp = head;
        if(head != NULL)
        {
            head = head->next;
            printf("delete num: %d, tid: %ld\n", tmp->num, pthread_self());
            free(tmp);
        }
        else
        {
            pthread_cond_wait(&cond, &mutex);  // 这时会暂时解锁mutex！
        }
        
        pthread_mutex_unlock(&mutex);
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
    pthread_cond_init(&cond, NULL);

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
