#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "intarr.h"

Stack stack_create(int size) {
    Stack* S = malloc(sizeof(Stack));
    S->arr = intarr_create(size);
    S->last = -1;
    return *S;
}

void stack_destroy(Stack *S) {
    intarr_destroy(S->arr);
    free(S);
}

int stack_isempty(Stack S) {
    return S.last == -1;
}

int stack_last(Stack S) {
    return S.arr->address[S.last];
}

void stack_add(Stack *S, int data) {
    S->last++;
    intarr_write(S->arr, S->last, data);
}

void stack_remove(Stack *S) {
    intarr_del(S->arr, S->last);
    S->last--;
}

void stack_print(Stack S) {
    printf("\n");
    for (int i = S.last; i >= 0; i--) 
        printf("%d ", S.arr->address[i]);
}