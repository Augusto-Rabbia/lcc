#ifndef _RWLOCK_A_H
#define _RWLOCK_A_H
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    pthread_mutex_t mutexRd;
    pthread_mutex_t mutexWr;
    int nLectores;
} RW;

RW* rw_init();

void rw_destr(RW* rw);

void reader_lock(RW* rw);

void writer_lock(RW* rw);

void reader_unlock(RW* rw);

void writer_unlock(RW* rw);
#endif