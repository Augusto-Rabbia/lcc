#include "CStack.h"

CStack* cstack_create() {
    CStack* s = (CStack*) malloc(sizeof(CStack));
    pthread_mutex_init(&(s->mutex), NULL);
    return s;
}

void cstack_push(CStack s, int data) {
    pthread_mutex_lock(&(s.mutex));
    push(&(s.stack), data);
    pthread_mutex_unlock(&(s.mutex));
}
int cstack_pop(CStack s) {
    pthread_mutex_lock(&(s.mutex));
    int ret = pop(&(s.stack));
    pthread_mutex_unlock(&(s.mutex));
    return ret;
}



