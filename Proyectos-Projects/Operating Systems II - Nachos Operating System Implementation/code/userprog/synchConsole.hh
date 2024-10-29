// TODO: Documentar

#ifndef NACHOS_FILESYS_SYNCHCONSOLEHH
#define NACHOS_FILESYS_SYNCHCONSOLEHH

#include "threads/lock.hh"
#include "threads/semaphore.hh"
#include "filesys/file_system.hh"
#include "filesys/file_system.hh"
#include "machine/console.hh"

class SynchConsole {
public:

    SynchConsole();

    ~SynchConsole();


    char GetChar();
    void PutChar(char c);

    void ReadAvail();
    void WriteDone();

private:
    Console* console;
    Semaphore* readSemaphore;
    Semaphore* writeSemaphore;
    Lock *writeLock;
    Lock *readLock; 
};
#endif