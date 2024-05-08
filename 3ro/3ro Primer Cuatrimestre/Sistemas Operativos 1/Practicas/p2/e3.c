#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>


volatile int x = 0;
volatile int y = 0;
void * wr(void *arg) { x = 123; y = 1; }
void * rd(void *arg) {
    while (!y);
    assert(x == 123);
}

int main() {
    pthread_t t0, t1;
    pthread_create(&t0, NULL, wr, NULL);
    pthread_create(&t1, NULL, rd, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    return 0;
}

// Falla si cambia el orden de las instrucciones el procesador.