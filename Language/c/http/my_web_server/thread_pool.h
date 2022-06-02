/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-05-18 10:43:24
 * @LastEditors: ruog__
 * @LastEditTime: 2022-05-18 10:46:12
 */
#include <pthread.h>
#include <stdio.h>
struct job
{
    void *(*callback_function)(void *arg);
    void *arg;
    struct job *next;
};

struct threadpool
{
    int thread_num;
    int queue_max_num;
    struct job *head;
    struct job *tail;
    pthread_t *pthreads;
    pthread_mutex_t mutex;
    pthread_cond_t queue_empty;
    pthread_cond_t queue_not_empty;
    pthread_cond_t queue_not_full;
    int queue_cur_num;
    int queue_close;
    int pool_close;
};

struct threadpool *threadpool_init(int thread_num, int queue_max_num);

int threadpool_add_job(struct threadpool *pool, void *(*callback_function)(void *arg), void *arg);

int threadpool_destroy(struct threadpool *pool);

void *threadpool_function(void *arg);
