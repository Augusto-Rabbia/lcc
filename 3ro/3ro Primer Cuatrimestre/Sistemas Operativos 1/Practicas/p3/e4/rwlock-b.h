#ifndef _RWLOCK_A_H
#define _RWLOCK_A_H
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    pthread_mutex_t mutexRd;
    pthread_mutex_t mutexWr;
    pthread_mutex_t mutexPeticionEntrar;
    pthread_cond_t cv;
    int nLectores;
    int escritor;
} RW;

RW* rw_init();

void rw_destr(RW* rw);

void reader_lock(RW* rw);

void writer_lock(RW* rw);

void reader_unlock(RW* rw);

void writer_unlock(RW* rw);
#endif