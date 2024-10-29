/// Routines for synchronizing threads.
///
/// The implementation for this primitive does not come with base Nachos.
/// It is left to the student.
///
/// When implementing this module, keep in mind that any implementation of a
/// synchronization routine needs some primitive atomic operation.  The
/// semaphore implementation, for example, disables interrupts in order to
/// achieve this; another way could be leveraging an already existing
/// primitive.
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "condition.hh"
#include "system.hh"


Condition::Condition(const char *debugName, Lock *conditionLock) {
    this->name = debugName;
    this->condLock = conditionLock;
    this->broadcasterSemaphore = new Semaphore("broadcasterSemaphore", 0);
    this->waitersSemaphore = new Semaphore("waitersSemaphore", 0);
    this->waiters = 0;
    this->waitersLock = new Lock(debugName);
}

Condition::~Condition() {
    delete broadcasterSemaphore;
    delete waitersSemaphore;
}

const char *
Condition::GetName() const {
    return this->name;
}

void
Condition::Wait() {
    this->waitersLock->Acquire();
    DEBUG('s', "[Cond] Thread %s waits on condition %s\n", currentThread->GetName(), this->GetName());
    this->waiters++;
    this->waitersLock->Release();

    this->condLock->Release();

    this->waitersSemaphore->P();
    this->broadcasterSemaphore->V();

    this->condLock->Acquire();
    DEBUG('s', "[Cond] Thread %s awakes on condition %s\n", currentThread->GetName(), this->GetName());
}

void
Condition::Signal() {
    this->waitersLock->Acquire();
    if (waiters > 0) {
        this->waitersSemaphore->V();
        this->waiters--;
    }
    DEBUG('s', "[Cond] Thread %s signals on condition %s\n", currentThread->GetName(), this->GetName());
    this->waitersLock->Release();
}

void
Condition::Broadcast() {
    this->waitersLock->Acquire();
    DEBUG('s', "[Cond] Thread %s broadcasts on condition %s\n", currentThread->GetName(), this->GetName());
    for (int i = 0; i < waiters; this->waitersSemaphore->V(), i++);
    while (waiters > 0) {
        this->broadcasterSemaphore->P();
        this->waiters--;
    }
    this->waitersLock->Release();
}
