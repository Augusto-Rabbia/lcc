#include "synchConsole.hh"

static void
ConsoleReadAvail(void* arg)
{
    ASSERT(arg != nullptr);
    SynchConsole* con = (SynchConsole*) arg;
    con->ReadAvail();
}
static void
ConsoleWriteDone(void* arg)
{
    ASSERT(arg != nullptr);
    SynchConsole* con = (SynchConsole*) arg;
    con->WriteDone();
}

// TODO: Documentar
SynchConsole::SynchConsole()
{
    readSemaphore = new Semaphore("synchConsole read sem", 0);
    writeSemaphore = new Semaphore("synchConsole write sem", 0);
    readLock = new Lock("synchConsole read lock");
    writeLock = new Lock("synchConsole write lock");
    console = new Console(nullptr, nullptr, ConsoleReadAvail, ConsoleWriteDone, this);
}

// TODO: Documentar
SynchConsole::~SynchConsole()
{
    delete readLock;
    delete writeLock;
    delete readSemaphore;
    delete writeSemaphore;
    delete console;
}

// TODO: Documentar
char
SynchConsole::GetChar()
{
    readLock->Acquire();
    readSemaphore->P();
    char charRead = console->GetChar();
    readLock->Release();
    return charRead;
}

// TODO: Documentar
void
SynchConsole::PutChar(char c)
{
    writeLock->Acquire();  // only one disk I/O at a time
    console->PutChar(c);
    writeSemaphore->P();   // Char written successfully
    writeLock->Release();
}

// TODO: Documentar
void
SynchConsole::ReadAvail()
{
    readSemaphore->V();
}

// TODO: Documentar
void
SynchConsole::WriteDone()
{
    writeSemaphore->V();
}