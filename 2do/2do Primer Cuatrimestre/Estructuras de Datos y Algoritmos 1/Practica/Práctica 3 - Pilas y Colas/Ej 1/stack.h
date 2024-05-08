#include "intarr.h"
#ifndef __stack_h__
#define __stack_h__

typedef struct _SNode {
    int data;
    struct _SNode *next;
} SNode;

typedef SNode* SStack;

typedef struct _Stack {
    intarr *arr;
    int last;
} Stack;

Stack stack_create(int size);

void stack_destroy(Stack *S);

int stack_isempty(Stack S);

int stack_last(Stack S);

void stack_add(Stack *S, int data);

void stack_remove(Stack *S);

void stack_print(Stack S);

#endif