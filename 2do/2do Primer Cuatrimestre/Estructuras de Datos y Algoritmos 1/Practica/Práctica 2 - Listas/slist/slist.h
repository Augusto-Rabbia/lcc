#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (int dato);

typedef void (*pFunc) (int num1, int num2);

typedef struct _SNodo {
  int dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

/**
 * Devuelve una lista vacía.
 */
SList slist_crear();

/**
 * Destruccion de la lista.
 */
void slist_destruir(SList lista);

/**
 * Determina si la lista es vacía.
 */
int slist_vacia(SList lista);

/**
 * Agrega un elemento al final de la lista.
 */
SList slist_agregar_final(SList lista, int dato);

/**
 * Agrega un elemento al inicio de la lista.
 */
SList slist_agregar_inicio(SList lista, int dato);

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void slist_recorrer(SList lista, FuncionVisitante visit);

int slist_len(SList list);

void slist_concat(SList *l1, SList l2);

void slist_insert(SList *l1, int pos, int data);

void slist_del(SList* l1, int pos);

int slist_contains(SList l1, int data);

SList slist_intersect(SList l1, SList l2);

SList slist_reverso(SList l1);

SList slist_intercalar(SList l1, SList l2);

#endif /* __SLIST_H__ */
