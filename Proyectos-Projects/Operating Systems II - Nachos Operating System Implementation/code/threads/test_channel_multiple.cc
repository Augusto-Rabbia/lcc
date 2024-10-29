#include "test_channel_multiple.hh"

#include <iostream>

#include "system.hh"
#include "channel.hh"

const int N_MSJS = 19;


void SenderThread(void* args) {
    Channel* chann = (Channel*) args;

    int x; 
    std::cout << currentThread->GetName() << " sends: ";
    std::cin >> x;

    chann->Send(x);

}

void ReceiverThread(void* args) {
    Channel* chann = (Channel*) args;

    int y;
    chann->Receive(&y);
    std::cout << "Receiver "<<currentThread->GetName() << " received: " << y << std::endl;
}


void
ThreadTestChannelMultiple() {

   Channel* chann = new Channel("Multiple S & R test");

   char* sNames[N_MSJS];
   char* rNames[N_MSJS];
   Thread* threadsS[N_MSJS];
   Thread* threadsR[N_MSJS];

    for (int i = 0; i < N_MSJS; i++) {
        sNames[i] = new char[100];
        sprintf(sNames[i], "%d%s", i+1, i == 0 ? "st S" : i == 1 ? "nd S" : i == 2 ? "rd S" : "th S");
        printf("Thread %s created\n", sNames[i]);
        Thread *newThread = new Thread(sNames[i], true);
        threadsS[i] = newThread;
        newThread->Fork(SenderThread, chann);
    }

    for (int i = 0; i < N_MSJS; i++) {
        rNames[i] = new char[100];
        sprintf(rNames[i], "%d%s", i+1, i == 0 ? "st R" : i == 1 ? "nd R" : i == 2 ? "rd R" : "th R");
        printf("Thread %s created\n", rNames[i]);
        Thread *newThread = new Thread(rNames[i], true);
        threadsR[i] = newThread;
        newThread->Fork(ReceiverThread, chann);
    }



    for (int i = 0; i < N_MSJS; i++) {
        printf("Joining %s\n", threadsS[i]->GetName());
        threadsS[i]->Join();
    }

    for (int i = 0; i < N_MSJS; i++) {
        printf("Joining %s\n", threadsR[i]->GetName());
        threadsR[i]->Join();
    }



    for (int i = 0; i < N_MSJS; ++i) {
        if (sNames[i] != nullptr) {
            delete[] sNames[i];
        }
    }

    for (int i = 0; i < N_MSJS; ++i) {
        if (rNames[i] != nullptr) {
            delete[] rNames[i];
        }
    }

    std::cout << "TEST FINISHED" << std::endl;
}
