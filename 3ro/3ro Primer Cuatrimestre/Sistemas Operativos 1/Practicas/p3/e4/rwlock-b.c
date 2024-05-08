#include "rwlock-b.h"

RW* rw_init() {
    RW* rw = (RW*) malloc(sizeof(RW));
    pthread_mutex_init(&(rw->mutexRd), NULL);
    pthread_mutex_init(&(rw->mutexWr), NULL);
    pthread_mutex_init(&(rw->mutexPeticionEntrar), NULL);
    pthread_cond_init(&(rw->cv), NULL);
    rw->nLectores = 0;
    rw->escritor = 0;
    return rw;
}

void rw_destr(RW* rw) {
    pthread_mutex_destroy(&(rw->mutexRd));
    pthread_mutex_destroy(&(rw->mutexWr));
    pthread_mutex_destroy(&(rw->mutexPeticionEntrar));
    pthread_cond_destroy(&(rw->cv));
    free(rw);
}

void reader_lock(RW* rw) {
    pthread_mutex_lock(&(rw->mutexPeticionEntrar));
    while (rw->escritor == 1)
        pthread_cond_wait(&(rw->cv), &(rw->mutexPeticionEntrar));
    pthread_mutex_unlock(&(rw->mutexPeticionEntrar));
    pthread_mutex_lock(&(rw->mutexRd));
    if (rw->nLectores == 0) 
        pthread_mutex_lock(&(rw->mutexWr));
    rw->nLectores++;
    pthread_mutex_unlock(&(rw->mutexRd));
}

void writer_lock(RW* rw) {
    pthread_mutex_lock(&(rw->mutexPeticionEntrar));
    pthread_mutex_lock(&(rw->mutexWr));
    rw->escritor = 1;
}

void reader_unlock(RW* rw) {
    pthread_mutex_lock(&(rw->mutexRd));
    rw->nLectores--;
    if (rw->nLectores == 0)
        pthread_mutex_unlock(&(rw->mutexWr));
    pthread_mutex_unlock(&(rw->mutexRd));
}

void writer_unlock(RW* rw) {
    rw->escritor = 0;
    pthread_mutex_unlock(&(rw->mutexPeticionEntrar));
    pthread_cond_broadcast(&(rw->cv));
    pthread_mutex_unlock(&(rw->mutexWr));    
}
