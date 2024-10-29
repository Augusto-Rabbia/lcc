#ifndef NACHOS_THREADS_CHANNEL_HH
#define NACHOS_THREADS_CHANNEL_HH

#include "condition.hh"
#include "lock.hh"

class Channel {
    // Data members
    private:
    
    Lock* ipcLock;
    Condition* ipcCondSend;
    Condition* ipcCondRecv;

    Lock* senderLock;
    Lock* receiverLock;

    
    enum BufferState {NO_MESSAGE, MESSAGE_SENT, MESSAGE_RECEIVED};


    int*            buffer;
    BufferState     buffState;
    const char*     name;


    // Member functions
    public:

    Channel(const char* name);

    ~Channel();

    const char* GetName();

    void Send(int message);
    void Receive(int* buffer);

    private:
};




#endif // NACHOS_THREADS_CHANNEL_HH
