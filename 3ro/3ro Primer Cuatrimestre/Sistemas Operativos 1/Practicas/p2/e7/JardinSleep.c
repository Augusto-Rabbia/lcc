#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N_VISITANTES 10


int visitantes = 0;

void * proc(void *arg) {
    int i;
    int id = arg - (void*)0;
    for (i = 0; i < N_VISITANTES; i++) {
        int c;
        if(id == 0 && i == N_VISITANTES - 1)
            sleep(2);
        
        c = visitantes;
        if(id == 0 && i == N_VISITANTES - 1)
            sleep(2);
        if(id == 0 && i == 0)
            sleep(1);
        if(id == 1 && i == 0)
            sleep(2);
        visitantes = c + 1;
        /* sleep? */
        if(id == 1 && i == 1)
            sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t m1, m2;
    pthread_create(&m1, NULL, proc, NULL);
    pthread_create(&m2, NULL, proc, (void*)1);
    
    // hacer join
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("VISITANTES:%d\n", visitantes);    
    return 0;
}