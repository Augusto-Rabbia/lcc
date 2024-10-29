/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "thread_test_prod_cons.hh"
#include "lock.hh"
#include "condition.hh"
#include "system.hh"
#include "thread.hh"
#include <stdio.h>



static Condition*   full;   //< Mutex de las variables de condición.
static Condition*   empty;  //< Condición donde se quedará esperando el PRODUCTOR cuando el buffer esté lleno.
static Lock*        mutex; //< Condición donde se quedará esperando el CONSUMIDOR cuando el buffer esté vacío.

static const int   BUFFER_SIZE = 3;     // Tamaño del buffer
static const int   MAX_ITEMS = 1000;    // Cantidad de items a producir 
static int buffer[3] = {-1};
static int         currentSlot;         // Posición del buffer que se rellenará. El hilo productor coloca un item y sube contador de slot. El hilo conumidor baja el contador de slot y consume el item. 

static bool producerFinished; // Bandera que indica la teminación del hilo productor
static bool consumerFinished; // Bandera que indica la teminación del hilo consumidor


void Consumer(void *n_) {
    for (int i = 1; i <= MAX_ITEMS; ++i) {
        mutex->Acquire();
        while (currentSlot <= 0) {
            printf("Consumidor esperando (buffer vacío)\n");
            empty->Wait();
        }

        ASSERT(currentSlot > 0);
        currentSlot--;
        printf("Consumidor consume: %d en %d\n", buffer[currentSlot], currentSlot);
        buffer[currentSlot] = -1;


        full->Signal();

        mutex->Release();
    }

    consumerFinished = true;
}

void Producer(void *n_) {
    for (int i = 1; i <= MAX_ITEMS; ++i) {
        mutex->Acquire();
        while (currentSlot >= BUFFER_SIZE) {
            printf("Productor esperando (buffer lleno)\n");
            full->Wait();
        }

        ASSERT(currentSlot < BUFFER_SIZE);

        buffer[currentSlot++] = i;
        printf("Productor produce: %d en %d\n", i, currentSlot-1);
        
        empty->Signal();

        mutex->Release();
    }

    producerFinished = true;
}


void
ThreadTestProdCons() {
    // Inicializamos las primitivas de sincronización
    mutex = new Lock("Mutex prod-cons problem");    
    full = new Condition("Full Buffer Cond", mutex); 
    empty = new Condition("Empty Buffer Cond", mutex);

    currentSlot = 0;

    producerFinished = false;
    consumerFinished = false;

    Thread* producer = new Thread("Producer thread");
    producer->Fork(Producer, NULL);

    Thread *consumer = new Thread("Consumer thread");
    consumer->Fork(Consumer, NULL);

    while (!(producerFinished && consumerFinished)) {
        currentThread->Yield();
    }
    
    delete mutex;
    delete full;
    delete empty;
}