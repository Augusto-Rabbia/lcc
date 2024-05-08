/*
    Primero que nada, para la correcta implementación del algoritmo, es necesario insertar
    fences para adaptarlo a las arquitecturas modernas y procesadores multicore.
        
    Este algoritmo tiene ciertos problemas. Para empezar, utiliza busy waiting, que lo hace
    menos eficiente que una solución con Mutex. Además, al sumarle threads, este se vuelve
    mucho más lento en comparación a todas las demás soluciones.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define N_VISITANTES 50000000
#define N_THREADS 2

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
    num[i] = 1 + max_num();
    eligiendo[i] = false;
    asm("mfence");

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

void* entran_visitantes1(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        lock(0);
        visitantes++;
        unlock(0);
    }
}

void* entran_visitantes2(void *arg) {
    for(int i = 0; i < N_VISITANTES; i++){
        lock(1);
        visitantes++;
        unlock(1);
    }
}

int main() {
    pthread_t m1, m2;
    pthread_create(&m1, NULL, entran_visitantes1, NULL);
    pthread_create(&m2, NULL, entran_visitantes2, NULL);
    

    // hacer join
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("VISITANTES:%d\n", visitantes);    
    return 0;
}
