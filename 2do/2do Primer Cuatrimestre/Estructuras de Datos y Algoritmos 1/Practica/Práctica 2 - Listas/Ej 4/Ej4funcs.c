#include <stdio.h>
#include <stdlib.h>
#include "Ej4funcs.h"

DList dlist_create() {
    DList new_list = {NULL};
    return new_list;
}

void cdlist_add_start(DList *l, int data) {
    DNode* new_node = malloc(sizeof(DNode));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    if(l->first == NULL)
    {
        l->first = new_node;
        l->last = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
        
    }
    else
    {
        l->last->next = new_node;
        new_node->prev = l->last;
        new_node->next = l->first;
        l->first->prev = new_node;
        l->first = new_node;
    }
}

void cdlist_recorrer(DList l, DListOrdenDeRecorrido dir, pFunc Func) {
    if(dir == DLIST_RECORRIDO_HACIA_ADELANTE) 
    {
        DNode* temp = l.first;
        for(; temp->next != l.first; temp = temp->next) {
            Func(temp->data);
        }
        Func(temp->data);
    }
    else 
    {
        DNode* temp = l.last;
        for(; temp != l.first; temp = temp->prev) {
            Func(temp->data);
        }
        Func(temp->data);
    }
}

void print_int(int n) {
    printf("%d ", n);
}