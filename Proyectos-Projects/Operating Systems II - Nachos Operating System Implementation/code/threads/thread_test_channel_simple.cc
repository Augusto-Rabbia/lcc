#include <stdio.h>

#include "thread_test_channel_simple.hh"
#include "channel.hh"
#include "system.hh"


static const int TEST_NUM_1 = 45; 
static const int TEST_NUM_2 = 99; 


static bool receiverThreadFinished = false;

void ReceiverTest(void* arg) {
  Channel* chan = (Channel*) arg;
  
  int receivedMessage;

  chan->Receive(&receivedMessage);

  ASSERT(receivedMessage == TEST_NUM_1);

  chan->Receive(&receivedMessage);
  ASSERT(receivedMessage == TEST_NUM_2);

    receiverThreadFinished = true;
}



void ThreadTestChannelSimple() {
    Channel* chan = new Channel("[Channel simple test]");

    Thread* receiverThread = new Thread("[Receiver thread - Channel simple test]");
    receiverThread->Fork(ReceiverTest, chan);

    chan->Send(TEST_NUM_1);
    chan->Send(TEST_NUM_2);

    while (!receiverThreadFinished) {
        currentThread->Yield();
    }
    
    delete chan;

    printf("Test finished\n");
}
