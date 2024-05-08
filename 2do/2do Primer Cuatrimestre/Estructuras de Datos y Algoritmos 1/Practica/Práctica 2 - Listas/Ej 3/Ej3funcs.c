#include <stdlib.h>
#include <stdio.h>
#include "Ej3funcs.h"

void print_int(int n) {
    printf("%d ", n);
}

void func_to_all_elements(SList list, pFunc fun) {
    for (SNode* temp = list.first; temp != NULL; temp = temp->next) {
        fun(temp->data);
    }
}

SList slist_crear() {
    SList l = {NULL};
    return l;
}

void slist_destroy(SList l1);

SList slist_add_end(SList list, int data) {
    SList newList = list;
    SNode* newNode = malloc(sizeof(SNode));
        newNode->data = data;
        newNode->next = NULL;
    if (list.last == NULL) 
    {
        newList.first = newNode;
        newList.last = newNode;
    }
    else 
    {
        newList.last->next = newNode;
        newList.last = list.last->next;
    }
    return newList;
}

SList slist_add_start(SList list, int data) {
    SList newList = list;
    SNode* newNode = malloc(sizeof(SNode));
        newNode->data = data;
        newNode->next = NULL;
    if (list.last == NULL) 
    {
        newList.first = newNode;
        newList.last = newNode;
    }
    else 
    {
        newNode->next = newList.first;
        newList.first = newNode;
    }
    return newList;
}