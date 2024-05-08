#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define N_VISITANTES 5000000
#define N_THREADS 4

volatile int num[N_THREADS] = {0};
volatile bool eligiendo[N_THREADS] = {false};

int max_num() {
    int max = num[0];
    for (int i = 1; i < N_THREADS; i++) {
        if (num[i] > max) {
            max = num[i];
        }
    }
    return max;
}

/* Código del hilo i */
void lock(int i) {
    /* Calcula el Numero de turno */
    eligiendo[i] = true;
    asm("mfence");
    num[i] = 1 + max_num();
    asm("mfence");
    eligiendo[i] = false;
    

    /* Compara con todos los hilos */
    for (int j = 0; j < N_THREADS; j++) {
        /* Si el hilo j está calculando su num, espera a que termine */
        while(eligiendo[j]);
        
        /* Si el hilo j tiene más prioridad, espera a que ponga su num a cero */
        /* j tiene más prioridad si su num de turno es más bajo que el de i, */
        /* o bien si es el mismo num y además j es menor que i */
        asm("mfence");
        while ((num[j] != 0) &&
            ((num[j] < num[i]) || ((num[j] == num[i]) && (j < i))));
    }
}
/* Sección crítica */
void unlock(int i) {
    num[i] = 0;
}

int visitantes = 0;


void* entran_visitantes(int id) {
    for(int i = 0; i < N_VISITANTES; i++){
        lock(id);
        visitantes++;
        unlock(id);
    }
}

int main() {
    
    omp_set_num_threads(N_THREADS);
    #pragma omp parallel
    {
        int my_id = omp_get_thread_num();
        entran_visitantes(my_id);
    }

    printf("VISITANTES:%d\n", visitantes);    
    return 0;
}
