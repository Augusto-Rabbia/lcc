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


#include "lock.hh"
#include "system.hh"

/// Dummy functions -- so we can compile our later assignments.

Lock::Lock(const char *debugName) {
    this->name = debugName;
    this->semaphore = new Semaphore(debugName, 1);
    this->currentLockOwner = nullptr;
    this->originalOwnerPriority = -1;
}

Lock::~Lock() {
    delete this->semaphore;
}

const char *
Lock::GetName() const {
    return this->name;
}

void
Lock::Acquire() {
    ASSERT(!IsHeldByCurrentThread());


    // Problema de  inversión de prioridades en semáforos:
    /*
    
    No se pueden heredar prioridades en semáforos ya que en este caso, los threads que levantan el semáforo no necesariamente serán aquellos que lo hayan bajado. Por lo tanto, si un thread de alta prioridad se bloquea en un semáforo que fue bajado por un thread de prioridad baja, no depende de que este último lo vuelva a levantar. En cambio, debería subirse la prioridad de los threads que llaman a Semaphore::V() en algún momento, pero no sabemos quienes son estos threads

    */

    if (this->currentLockOwner != nullptr){
        if (this->currentLockOwner->GetPriority() < currentThread->GetPriority()) {
            scheduler->Reeschedule(this->currentLockOwner, currentThread->GetPriority());
        }
    } else {
        this->originalOwnerPriority = currentThread->GetPriority();
    }

    this->semaphore->P();
    this->currentLockOwner = currentThread;
    DEBUG('s', "Lock [%s] acquired by thread [%s].\n", this->name, currentThread->GetName());
}

void
Lock::Release() {
    ASSERT(IsHeldByCurrentThread());
    DEBUG('s', "Lock [%s] released by thread [%s].\n", this->name, currentThread->GetName());
    currentThread->SetPriority(this->originalOwnerPriority);
    
    this->currentLockOwner = nullptr;
    this->originalOwnerPriority = -1;
    this->semaphore->V();
}

bool
Lock::IsHeldByCurrentThread() const {
    return this->currentLockOwner == currentThread;
}
