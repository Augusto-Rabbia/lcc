#ifndef __EJ3FUNCS_H__
#define __EJ3FUNCS_H__

typedef void (*pFunc) (int n);

typedef struct _SNode{
    int data;
    struct _SNode *next;
} SNode;

typedef struct SList {
    SNode *first;
    SNode *last;
} SList;

void print_int(int n);

void func_to_all_elements(SList list, pFunc fun);

SList slist_crear();

void slist_destroy(SList l1);

SList slist_add_start(SList list, int dato);

SList slist_add_end(SList list, int dato);

#endif
