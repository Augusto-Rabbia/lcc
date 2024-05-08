#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "n_vis.h"

int visitantes = 0;

void entran_visitantes() {
    for(int i = 0; i < N_VISITANTES; i++){
        visitantes++;
    }
}

int main()
{
    entran_visitantes();
    entran_visitantes();
    printf("VISITANTES:%d\n", visitantes);    
    return 0;
}