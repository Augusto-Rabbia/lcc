#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "n_vis.h"

// pthread_mutex es mejor que pthread_spinlock cuando se tienen que hacer los locks en operaciones
// costosas o largas, sino, es mejor el spinlock

int visitantes = 0;

pthread_mutex_t mutex;

void *entran_visitantes(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        pthread_mutex_lock(&mutex);
        // region critica
        visitantes++;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_mutex_init(&mutex, 0);
    pthread_t m1, m2;
    pthread_create(&m1, NULL, entran_visitantes, NULL);
    pthread_create(&m2, NULL, entran_visitantes, NULL);
    
    // hacer join
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("VISITANTES: %d\n", visitantes);    
    return 0;
}