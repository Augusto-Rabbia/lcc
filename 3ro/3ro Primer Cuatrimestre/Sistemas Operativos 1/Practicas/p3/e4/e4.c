#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
//#include "rwlock-a.h"
#include "rwlock-b.h"

#define M 7
#define N 2
#define ARRLEN 1024

int arr[ARRLEN];

RW* rwlock;

void * escritor(void *arg) {
    int i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        writer_lock(rwlock);
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
            arr[i] = num;
        writer_unlock(rwlock);
    }
    return NULL;
}

void * lector(void *arg) {
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        reader_lock(rwlock);
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
            break;
        }
        //for(int i = 0; i<10000000; i++);
        sleep(1);
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);
        reader_unlock(rwlock);
    }
    return NULL;
}

int main() {
    pthread_t lectores[M], escritores[N];
    rwlock = rw_init();
    int i;
    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}