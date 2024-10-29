#include "channel.hh"
#include "system.hh"
#include <stdio.h>

Channel::Channel(const char* name) {
    this->name = name;

    this->ipcLock         = new Lock("Channel ipcLock");
    this->ipcCondSend     = new Condition("Channel ipcCondSend", ipcLock);
    this->ipcCondRecv     = new Condition("Channel ipcCondRecv", ipcLock);

    this->senderLock      = new Lock("Channel senderLock");
    this->receiverLock    = new Lock("Channel receiverLock");

    this->buffState = NO_MESSAGE;
    this->buffer = new int;
}

Channel::~Channel() {
  delete this->ipcLock;
  delete this->ipcCondSend;
  delete this->ipcCondRecv;
  delete this->senderLock;
  delete this->receiverLock;
  delete this->buffer;
}

const char* Channel::GetName() {
    return this->name;
}

void Channel::Send(int message) {
    DEBUG('s', "[Channel::Send] Thread %s tries to SEND the message: [%d]\n", currentThread->GetName(), message);
    this->senderLock->Acquire();
    this->ipcLock->Acquire();
    ASSERT(this->buffState == NO_MESSAGE);

    *(this->buffer) = message;

    this->buffState = MESSAGE_SENT;


    this->ipcCondRecv->Signal();
    while (this->buffState != MESSAGE_RECEIVED) {
        this->ipcCondSend->Wait();
    }
    this->buffState = NO_MESSAGE;
    this->senderLock->Release();
    this->ipcLock->Release();
}

void Channel::Receive(int* buffer) {
    DEBUG('s', "[Channel::Receive] Thread %s tries to RECEIVE a message.\n", currentThread->GetName());
    this->receiverLock->Acquire();

    this->ipcLock->Acquire();

    while (this->buffState != MESSAGE_SENT) {
        this->ipcCondRecv->Wait();
    }

    ASSERT(this->buffState != NO_MESSAGE);

    if (buffer != nullptr) {
        *(buffer) = *(this->buffer);
    }
    
    this->buffState = MESSAGE_RECEIVED;
    this->ipcCondSend->Signal();
    this->receiverLock->Release();
    this->ipcLock->Release();
}
