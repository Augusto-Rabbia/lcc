#ifndef __SLIST_H__
#define __SLIST_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _SNode {
    char* str;
    int pasosRestantes;
    int idxGuardado; //idxEncontrada o idxMaxMod
    struct _SNode* next;
} SNode;
typedef SNode* SList;

/**
 * Inicializa una lista como NULL.
 */
SList slist_crear();

/**
 * Destruye una lista.
 */
void slist_destruir(SList list);

/**
 * Agrega un nodo con los datos insertados al final de la lista.
 */
SList slist_agregar_final(SList l, char* dato, int idxMaxMod, int cantCorrecciones);

/**
 * Agrega el nodo indicado al final de la lista.
 */
void slist_agregar_nodo_final(SList l, SNode* node);

#endif