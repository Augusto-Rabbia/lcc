#include <stdio.h>
#include <stdlib.h>
#include "intarr.h"
#include "stack.h"

int main(void) {
    Stack S1 = stack_create(3);
    printf("empty: %d", stack_isempty(S1));
    stack_add(&S1, 3);
    stack_add(&S1, 2);
    stack_add(&S1, 1);

    printf("\nempty: %d", stack_isempty(S1));

    printf("\n%d", stack_last(S1));
    stack_print(S1);
    stack_remove(&S1);
    printf("\n%d\n", stack_last(S1));
    
    stack_destroy(&S1);

    return 0;
}