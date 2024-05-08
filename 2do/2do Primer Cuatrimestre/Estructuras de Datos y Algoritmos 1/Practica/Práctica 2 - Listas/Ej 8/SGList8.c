#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SGList8.h"

SGList sglist_create() { return NULL; }

void sglist_destroy(SGList list, destroying_func d_f) {
    for(SGList temp = list; temp->next != NULL; temp = temp->next) {
        d_f(temp->data);
    }
}

int sglist_empty(SGList l) {
    return (l == NULL);
}

SGList sglist_add_at_start(SGList l, void* data) {
    SGNode* new_node = malloc(sizeof(SGNode));
    assert(new_node != NULL);
    new_node->data = data;
    new_node->next = l;
    return new_node;
}