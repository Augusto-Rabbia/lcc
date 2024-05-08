#include "rwlock-a.h"

RW* rw_init() {
    RW* rw = (RW*) malloc(sizeof(RW));
    pthread_mutex_init(&(rw->mutexRd), NULL);
    pthread_mutex_init(&(rw->mutexWr), NULL);
    rw->nLectores = 0;
    return rw;
}

void rw_destr(RW* rw) {
    pthread_mutex_destroy(&(rw->mutexRd));
    pthread_mutex_destroy(&(rw->mutexWr));
    free(rw);
}

void reader_lock(RW* rw) {
    pthread_mutex_lock(&(rw->mutexRd));
    if (rw->nLectores == 0) 
        pthread_mutex_lock(&(rw->mutexWr));
    rw->nLectores++;
    pthread_mutex_unlock(&(rw->mutexRd));
}

void writer_lock(RW* rw) {
    pthread_mutex_lock(&(rw->mutexWr));
}

void reader_unlock(RW* rw) {
    printf("%d\n", rw->nLectores);
    pthread_mutex_lock(&(rw->mutexRd));
    rw->nLectores--;
    if (rw->nLectores == 0)
        pthread_mutex_unlock(&(rw->mutexWr));
    pthread_mutex_unlock(&(rw->mutexRd));
}

void writer_unlock(RW* rw) {
    pthread_mutex_unlock(&(rw->mutexWr));    
}
