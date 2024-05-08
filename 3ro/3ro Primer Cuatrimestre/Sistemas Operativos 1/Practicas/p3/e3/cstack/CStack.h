#ifndef CSTACK_H
#define CSTACK_H

#include "stack_unbound.h"
#include <pthread.h>

typedef struct _CStack {
    struct StackNode* stack;
    pthread_mutex_t mutex;
} CStack;


CStack* cstack_create();

void cstack_push(CStack s, int data);

int cstack_pop(CStack s);





#endif