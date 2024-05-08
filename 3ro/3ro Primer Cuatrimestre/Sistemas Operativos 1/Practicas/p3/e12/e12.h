#ifndef _E12_H_
#define _E12_H_

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct _my_cond_t {
    sem_t semProtector; // Lo hace thread safe
    sem_t semaphore;
    int cantWaiting;
} my_cond_t;

void my_cond_wait(my_cond_t* c, pthread_mutex_t* m);

void my_cond_signal(my_cond_t* c);

void my_cond_broadcast(my_cond_t* c);
#endif