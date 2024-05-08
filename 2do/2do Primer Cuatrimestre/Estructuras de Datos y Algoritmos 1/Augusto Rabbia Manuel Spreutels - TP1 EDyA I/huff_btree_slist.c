#include <stdlib.h>
#include <stdio.h>
#include "huff_btree_slist.h"

/*     SLIST     */

SList slist_crear() {
  return NULL;
}

SList slist_agregar_inicio(SList lista, WeightedTree warbol) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->wtree = warbol;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

SList slist_agregar_final(SList lista, WeightedTree warbol) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->wtree = warbol;

  nuevoNodo->sig = NULL;

  if (lista == NULL)
    return nuevoNodo;

  SList nodo = lista;
  for (;nodo->sig != NULL;nodo = nodo->sig);
  /* ahora 'nodo' apunta al ultimo elemento en la lista */

  nodo->sig = nuevoNodo;
  return lista;
}

SList slist_insertar_ordenado(SList lista, WeightedTree wTreeToInsert) {
  if (lista == NULL || lista->wtree->peso > wTreeToInsert->peso) return slist_agregar_inicio(lista, wTreeToInsert);
  SList nodo = lista;
  SList nodoant = nodo;
  nodo = nodo->sig;
  for(; nodo != NULL; nodoant = nodo, nodo = nodo->sig) {
    if (wTreeToInsert->peso <= nodo->wtree->peso) {
      nodoant->sig = slist_agregar_inicio(nodo, wTreeToInsert);
      return lista;
    }
  }
  SList newNode = malloc(sizeof(SNodo));
  newNode->sig = NULL;
  newNode->wtree = wTreeToInsert;
  nodoant->sig = newNode;
  return lista;
}


/*     BTREE     */

BTree btree_conectar(int dato, BTree r, BTree l) {
    BTree root = malloc(sizeof(struct _BTNode));
    root->data = dato;
    root->right = r;
    root->left = l;
    return root;
}

void btree_destruir(BTree tree) {
  if (tree != NULL) {
    btree_destruir(tree->left);
    btree_destruir(tree->right);
    free(tree);
  }
}
