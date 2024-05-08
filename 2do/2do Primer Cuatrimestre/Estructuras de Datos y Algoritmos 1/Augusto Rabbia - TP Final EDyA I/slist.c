#include "slist.h"

SList slist_crear() { return NULL; }

void slist_destruir(SList list) {
    SNode *nodeToDel;
    while (list != NULL) {
        nodeToDel = list;
        list = list->next;
        free(nodeToDel->str);
        free(nodeToDel);
        nodeToDel = NULL;
  }
}

SList slist_agregar_final(SList l, char* dato, int idxMaxMod, int pasosRestantes) {
    SNode* newNode = malloc(sizeof(SNode));
    newNode->next = NULL;
    newNode->str = malloc(sizeof(char) * 256);
    strcpy(newNode->str, dato);
    newNode->idxGuardado = idxMaxMod;
    newNode->pasosRestantes = pasosRestantes;
    if (l == NULL)
        return newNode;
    else {
        SList temp;
        for (temp = l; temp->next != NULL; temp = temp->next);
        temp->next = newNode;
        return l;
    }
}

void slist_agregar_nodo_final(SList l, SNode* node) {
    SList temp;
    for(temp = l; temp->next != NULL; temp = temp->next);
    temp->next = node;
}
