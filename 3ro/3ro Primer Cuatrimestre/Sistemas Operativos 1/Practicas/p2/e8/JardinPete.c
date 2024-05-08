#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "n_vis.h"

int visitantes = 0;

int flags[2] = {0,0};   // si flag[i] = 1 significa que el thread i quiere
                        // entrar a la region critica
int turno = 0;

void *entran_visitantes0(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        flags[0] = 1;
        turno = 1;
        asm("mfence");
        while(flags[1] == 1 && turno == 1);
        
        // region critica
        visitantes++;
        flags[0] = 0;  // Libero la region critica
    }
}

void *entran_visitantes1(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        flags[1] = 1;
        turno = 2;
        asm("mfence");
        while(flags[0] == 1 && turno == 2);
        
        // region critica
        visitantes++;
        flags[1] = 0;  // Libero la region critica
    }
}

int main() {
    pthread_t m1, m2;
    pthread_create(&m1, NULL, entran_visitantes0, NULL);
    pthread_create(&m2, NULL, entran_visitantes1, NULL);
    
    // hacer join
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("VISITANTES: %d\n", visitantes);    
    return 0;
}