#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack_unbound.h"

// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

int main() {
    struct StackNode* root = NULL;
 
    push(&root, 10);
    push(&root, 20);
    push(&root, 30);
 
    printf("%d popped from stack\n", pop(&root));
 
    printf("Top element is %d\n", top(root));

    stackFree(root);
 
    return 0;
}