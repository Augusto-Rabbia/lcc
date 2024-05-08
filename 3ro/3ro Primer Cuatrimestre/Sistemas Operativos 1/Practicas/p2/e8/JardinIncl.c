#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "n_vis.h"

int visitantes = 0;


static inline void incl(int *p) {
    asm("incl %0" : "+m"(*p) : : "memory");
}

void *entran_visitantes(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        incl(&visitantes);
    }
}

int main()
{
    pthread_t m1, m2;
    pthread_create(&m1, NULL, entran_visitantes, NULL);
    pthread_create(&m2, NULL, entran_visitantes, NULL);
    
    // hacer join
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("VISITANTES:%d\n", visitantes);    
    return 0;
}