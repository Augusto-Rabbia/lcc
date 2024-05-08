#ifndef __Ej4funcs_h__
#define __Ej4funcs_h__

typedef struct _DNode {
    void* data;
    struct _DNode* next;
    struct _DNode* prev;
} DNode;

typedef struct _DList {
    DNode* first;
} DList;

typedef enum {
    DLIST_RECORRIDO_HACIA_ADELANTE,
    DLIST_RECORRIDO_HACIA_ATRAS
} DListOrdenDeRecorrido;

typedef void (*pFunc) (int num1);  

DList dlist_create();

void cdlist_add_start(DList *l, int data);

void cdlist_recorrer(DList l, DListOrdenDeRecorrido dir, pFunc Func);

void print_int(int n);

#endif