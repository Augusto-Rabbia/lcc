#include "slist.h"
#include <stdlib.h>
#include <stdio.h>

SList slist_crear() {
  return NULL;
}

void slist_destruir(SList lista) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    free(nodoAEliminar);
  }
}

int slist_vacia(SList lista) {
  return lista == NULL;
}

SList slist_agregar_final(SList lista, int dato) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;

  if (lista == NULL)
    return nuevoNodo;

  SList nodo = lista;
  for (;nodo->sig != NULL;nodo = nodo->sig);
  /* ahora 'nodo' apunta al ultimo elemento en la lista */

  nodo->sig = nuevoNodo;
  return lista;
}

SList slist_agregar_inicio(SList lista, int dato) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

void slist_recorrer(SList lista, FuncionVisitante visit) {
  for (SNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    visit(nodo->dato);
}

int slist_len(SList list) {
  int i = 0;
  if (list == NULL) 
  {
    return 0;
  }
  else 
  {
    for(SNodo* temp = list; temp->sig != NULL; temp = temp->sig, i++);
    return i+1;
  }
  
}

void slist_concat(SList *l1, SList l2) {
  if (*l1 == NULL) *l1 = l2;
  else 
  {
    SList temp = *l1;
    for(; temp->sig != NULL; temp = temp->sig);
    temp->sig = l2;
  }
}


void slist_insert(SList *l1, int pos, int data) {
  if (*l1 == NULL) 
  {
    *l1 = malloc(sizeof(SNodo));
    (*l1)->dato = data;
    (*l1)->sig = NULL;
  }
  else 
  {
    SList temp = *l1;
    for(int i = 1; temp->sig != NULL, i < pos; temp = temp->sig, i++);
    SNodo* newNode = malloc(sizeof(SNodo));
    newNode->dato = data;
    newNode->sig = temp->sig;
    temp->sig = newNode;
  }
}

void slist_del(SList *l1, int pos) {
  if (pos == 0) {
    SNodo* toDel = *l1;
    *l1 = (*l1)->sig;
    free(toDel);
  }
  else {
    SList temp = *l1;
    for(int i = 1; temp->sig != NULL, i < pos; temp = temp->sig, i++);
    if (temp->sig != NULL) 
    {
      SNodo* toDel = temp->sig;
      temp->sig = temp->sig->sig;
      free(toDel);
    }
    else 
    {
      printf("list index too high!");
    }
  }
  
}

int slist_contains(SList l1, int data) {
  for(SList temp = l1; temp->sig != NULL; temp = temp->sig) {
    if (temp->dato == data) return 1;
  }
  return 0;
}

SList slist_intersect(SList l1, SList l2) {
  SList newList = NULL;

  for(SList temp = l1; temp != NULL; temp = temp->sig) {
    if (slist_contains(l2, temp->dato)) 
    {
      newList = slist_agregar_inicio(newList, temp->dato);
    }
  }
  return newList;
}

SList slist_reverso(SList l1) {
  SList t = l1, t1, t2 = NULL, t3, t4, t5;
  for(; t->sig != NULL; t = t->sig) {
    //printf("%d, ", t->dato);
    if (t2 != NULL) {
      t1 = t;
      printf("%d, ", t->dato);
      printf("%d, ", t3->dato);
      t3->sig = t2;
      t2 = t1->sig;
      t1->sig = t3;
      t3 = t2->sig;
      t = t3;
    }
    else {
      t1 = t;
      t2 = t->sig;
      if (t2->sig != NULL) 
      {
        t3 = t2->sig;
        t1->sig = NULL;
        t2->sig = t1;
        t = t3;
      }
      else
      {

      }
    }
  }
  return t;
}

SList slist_intercalar(SList l1, SList l2) {
  if (l1 == NULL) return l2;
  if (l2 == NULL) return l1;

  SList newList, temp = l1, temp2 = l2, t1C, t2C;
  for(; temp->sig != NULL && temp2->sig != NULL; temp = temp->sig, temp2 = temp2->sig) {
    newList = slist_agregar_final(newList, temp->dato);
    newList = slist_agregar_final(newList, temp2->dato);
  }
  if (temp->sig == NULL)
  {
    slist_concat(&newList, temp2);
    return newList;
  }
  else if (temp2->sig == NULL)
  {
    slist_concat(&newList, temp);
    return newList;
  }
  else return newList;
}


