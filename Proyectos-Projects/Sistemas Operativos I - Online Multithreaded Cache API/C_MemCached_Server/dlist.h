#ifndef __DList_H__
#define __DList_H__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

typedef struct _DNode {
  char *clave, *valor;
  size_t lenClave, lenValor;
  bool binario;
  struct _DNode *next;
  struct _DNode *prev;
  struct _DNode *older;
  struct _DNode *newer;
} DNode;

typedef DNode *DList;

typedef struct _SHistoryQueue {
  pthread_mutex_t historyMutex;
  DNode* newest;
  DNode* oldest;
} HistoryQueue;

// Inserta un nodo al comienzo de la hq
void hq_insertar(HistoryQueue* hq, DNode* node);

// Únicamente elimina un nodo de la hq, sin liberar su memoria
void hq_eliminar(HistoryQueue *hq, DList* list, DNode* node);

// Mueve un nodo al comienzo de la hq
void hq_update(HistoryQueue *hq, DNode* node);

/**
 * Destruccion de la lista.
 */
void dlist_destruir(DList lista);

/**
 * Destruye un nodo. 
 * NO se encarga de modificar sus variables relacionadas a la hq.
 */
void dnode_destruir(DNode* nodo);
/**
 * Agrega un elemento al inicio de la lista.
 */
DList dlist_agregar_inicio(DList list, char* clave, size_t lenV, char* valor, size_t lenC, bool bin);

/**
 * Retorna el dato de la lista que coincida con el dato buscado, 
 * o NULL si el dato buscado no se encuentra en la tabla.
*/
DList dlist_buscar_y_retornar(DList list, char* clave, size_t lenC);



typedef struct _ENode {
  int data;
  struct _ENode *next;
} ENode;

typedef struct _EList {
  pthread_mutex_t listMutex;
  ENode *first;
  ENode *last;
} EList;

void elist_agregar_final(EList *list, int dato);

void elist_elim(EList *list, int dato);

void elist_destroy(EList list);

#endif /* __DList_H__ */