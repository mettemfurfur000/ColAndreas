
#include "include/threaded.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void (*task)(void *);
    void *arg;
} ThreadTask;

typedef struct
{
    ThreadTask *tasks;
    int capacity;
    int size;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} ThreadPool;

static ThreadPool *pool = NULL;
static pthread_t *threads = NULL;
static int num_threads = 0;
static int is_running = 0;

static void *worker_thread(void *arg)
{
    while (is_running)
    {
        pthread_mutex_lock(&pool->mutex);
        while (pool->size == 0 && is_running)
        {
            pthread_cond_wait(&pool->not_empty, &pool->mutex);
        }
        if (!is_running)
        {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }
        ThreadTask task = pool->tasks[pool->front];
        pool->front = (pool->front + 1) % pool->capacity;
        pool->size--;
        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->mutex);
        task.task(task.arg);
    }
    return NULL;
}

int init_thread_pool(int thread_count)
{
    if (pool != NULL)
    {
        return 0; // Already initialized
    }
    pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (pool == NULL)
    {
        return -1;
    }
    pool->capacity = thread_count * 2;
    pool->tasks = (ThreadTask *)malloc(sizeof(ThreadTask) * pool->capacity);
    if (pool->tasks == NULL)
    {
        free(pool);
        return -1;
    }
    pool->size = 0;
    pool->front = 0;
    pool->rear = 0;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    if (threads == NULL)
    {
        free(pool->tasks);
        free(pool);
        return -1;
    }
    num_threads = thread_count;
    is_running = 1;

    for (int i = 0; i < thread_count; i++)
    {
        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0)
        {
            destroy_thread_pool();
            return -1;
        }
    }
    return 0;
}

int add_work(void (*task)(void *), void *arg)
{
    if (pool == NULL || !is_running)
    {
        return -1;
    }
    pthread_mutex_lock(&pool->mutex);
    while (pool->size == pool->capacity && is_running)
    {
        pthread_cond_wait(&pool->not_full, &pool->mutex);
    }
    if (!is_running)
    {
        pthread_mutex_unlock(&pool->mutex);
        return -1;
    }
    pool->tasks[pool->rear].task = task;
    pool->tasks[pool->rear].arg = arg;
    pool->rear = (pool->rear + 1) % pool->capacity;
    pool->size++;
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}

void destroy_thread_pool(void)
{
    if (pool == NULL)
    {
        return;
    }
    pthread_mutex_lock(&pool->mutex);
    is_running = 0;
    pthread_cond_broadcast(&pool->not_empty);
    pthread_cond_broadcast(&pool->not_full);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);
    free(pool->tasks);
    free(pool);
    free(threads);

    pool = NULL;
    threads = NULL;
    num_threads = 0;
}
