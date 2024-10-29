/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#include "thread_test_join.hh"
#include "system.hh"
#include <stdio.h>
#include <string.h>

int terminado = 0;

void LlenarBuffer(void *x) {
    currentThread->Yield();                      // Para asegurarnos de que espera
    DEBUG('j', "Entrando a LlenarBuffer\n");     // J por Join
    terminado = 1;
    DEBUG('j', "Child thread terminando\n");
}

void ThreadTestJoin() {
    Thread *child = new Thread("Child", true);
    DEBUG('j', "ThreadMain forkeando\n"); 
    child->Fork(LlenarBuffer, nullptr);
    DEBUG('j', "ThreadMain waiting for the child\n");
    child->Join();
    printf("Termina el master: %d\n", terminado);
}