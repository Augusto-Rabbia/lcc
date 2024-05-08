#ifndef __SGList8_h__
#define __SGList8_h__

typedef struct _SGNode {
    void* data;
    struct _SGNode* next;
} SGNode;

typedef SGNode* SGList;

typedef void (*destroying_func) (void* data);

SGList sglist_create();

void sglist_destroy(SGList list, destroying_func d_f);

SGList sglist_add_at_start(SGList l, void* data);

#endif