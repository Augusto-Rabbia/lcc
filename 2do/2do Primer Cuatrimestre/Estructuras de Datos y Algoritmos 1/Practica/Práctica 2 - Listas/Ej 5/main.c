#include <stdio.h>
#include <stdlib.h>
#include "Ej4funcs.h"



int main(void) {

    DList l1 = dlist_create();
    cdlist_add_start(&l1, 5);
    cdlist_add_start(&l1, 4);
    cdlist_add_start(&l1, 3);
    cdlist_add_start(&l1, 2);
    cdlist_add_start(&l1, 1);

    //printf("%d", l1.first->prev->prev->data);

    DListOrdenDeRecorrido dir = DLIST_RECORRIDO_HACIA_ATRAS;

    cdlist_recorrer(l1, dir, print_int);

    return 0;
}