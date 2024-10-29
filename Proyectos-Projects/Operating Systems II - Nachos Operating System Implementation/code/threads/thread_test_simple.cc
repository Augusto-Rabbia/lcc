/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "thread_test_simple.hh"
#include "system.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.hh"

#define N_THREADS 5 // Define the number of threads to be deployed

/// Loop 10 times, yielding the CPU to another ready thread each iteration.
///
/// * `name` points to a string with a thread name, just for debugging
///   purposes.

Thread* threads[N_THREADS] = {0};


#ifdef SEMAPHORE_TEST
    static Semaphore* semaphore;
#endif // SEMAPHORE_TEST


void
SimpleThread(void *name_)
{
    // If the lines dealing with interrupts are commented, the code will
    // behave incorrectly, because printf execution may cause race
    // conditions.

    #ifdef SEMAPHORE_TEST
        semaphore->P();
    #endif // SEMAPHORE_TEST

    for (unsigned num = 0; num < 10; num++) {
        printf("*** Thread `%s` is running: iteration %u\n", currentThread->GetName(), num);
        currentThread->Yield();
    }

    printf("!!! Thread `%s` has finished SimpleThread\n", currentThread->GetName());
    #ifdef SEMAPHORE_TEST
        semaphore->V();
    #endif // SEMAPHORE_TEST
}

/// Set up a ping-pong between several threads.
///
/// Do it by launching one thread which calls `SimpleThread`, and finally
/// calling `SimpleThread` on the current thread.
void
ThreadTestSimple()
{
    #ifdef SEMAPHORE_TEST
        semaphore = new Semaphore("testSemaphore", 3);
    #endif // SEMAPHORE_TEST

    char* name[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        name[i] = new char[10];
        sprintf(name[i], "%d%s", i+2, i == 0 ? "nd" : i == 1 ? "rd" : "th");
        Thread *newThread = new Thread(name[i], true);
        threads[i] = newThread;
        newThread->Fork(SimpleThread, name);
    }

    //the "main" thread also executes the same function
    SimpleThread(NULL);

    //Wait for the 2nd, 3rd, ..., nth threads to finish if needed
    for (int i = 0; i < N_THREADS; i++) {
        threads[i]->Join();
    }

    #ifdef SEMAPHORE_TEST
        delete semaphore;
    #endif // SEMAPHORE_TEST


    for (int i = 0; i < N_THREADS; ++i) {
        if (name[i] != nullptr) {
            delete[] name[i];
        }
    }


    printf("Test finished\n");
}
