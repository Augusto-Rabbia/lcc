#include <stdlib.h>
#include <stdio.h>
#include "Ej3funcs.h"

int main(void) {
    SList l1 = slist_crear();
    l1 = slist_add_end(l1, 2);
    l1 = slist_add_end(l1, 3);
    l1 = slist_add_end(l1, 4);
    l1 = slist_add_end(l1, 5);
    l1 = slist_add_start(l1, 1);
    func_to_all_elements(l1, print_int);
    printf("\nFirst: %d\nLast: %d\n", l1.first->data, l1.last->data);

    return 0;
}
