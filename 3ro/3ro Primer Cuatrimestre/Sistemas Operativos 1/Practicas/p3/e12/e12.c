#include "e12.h"

void my_cond_init(my_cond_t* c) {
    sem_init(&(c->semProtector), 0, 1);
    sem_init(&(c->semaphore), 0, 0);
    c->cantWaiting = 0;
}

void my_cond_wait(my_cond_t* c, pthread_mutex_t *mutex) {
    sem_wait(&(c->semProtector));
    c->cantWaiting++;
    sem_post(&(c->semProtector));
    
    pthread_mutex_unlock(mutex);
    sem_wait(&(c->semaphore));
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t* c) {
    sem_wait(&(c->semProtector));
    if (c->cantWaiting > 0)
        c->cantWaiting--;
    sem_post(&(c->semaphore));
    sem_post(&(c->semProtector));
}

void my_cond_broadcast(my_cond_t* c) {
    sem_wait(&(c->semProtector));
    for (int i = c->cantWaiting; i > 0; i--)
        sem_post(&(c->semaphore));
    c->cantWaiting = 0;
    sem_post(&(c->semProtector));
}

