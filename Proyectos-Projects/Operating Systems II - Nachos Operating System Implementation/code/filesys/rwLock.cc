#include "rwLock.hh"
#include "threads/condition.hh"
#include "threads/lock.hh"
#include "threads/system.hh"

RWLock::RWLock(const char *debugName) {
    this->lck = new Lock(debugName);
    this->cnd = new Condition(debugName, this->lck);
}

RWLock::~RWLock() {
    delete cnd;
    delete lck;
}

void RWLock::AcquireReader() {
    this->lck->Acquire();
    while (writersWaiting > 0
            || (currWriter != nullptr && currWriter != currentThread)) {
        this->cnd->Wait();
    }
    
    ++activeReaders;
    this->lck->Release();
}

void RWLock::ReleaseReader() {
    ASSERT(this->lck != nullptr);
    this->lck->Acquire();
    activeReaders -= 1;
    
    if (activeReaders == 0) {
        this->cnd->Broadcast();
    }
    
    this->lck->Release();
}

void RWLock::AcquireWriter() {
    this->lck->Acquire();
    if (currWriter != currentThread) {
        writersWaiting += 1;
    }
    
    while (activeReaders > 0 
            || (currWriter != nullptr && currWriter != currentThread)) {
        this->cnd->Wait();
    }
    
    if (currWriter != currentThread) { --writersWaiting; }
    currWriter = currentThread;
    writeDepth += 1;
    this->lck->Release();
}

void RWLock::ReleaseWriter() {
    this->lck->Acquire();
    writeDepth -= 1;
 
    if (writeDepth == 0) {
        currWriter = nullptr;
        this->cnd->Broadcast();
    }
    this->lck->Release();
}