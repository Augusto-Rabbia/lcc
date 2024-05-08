#include "btree.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

struct _BTNodo {
  int data;
  struct _BTNodo *left;
  struct _BTNodo *right;
};

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear() { return NULL; }

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo);
  }
}

/**
 * Indica si el árbol es vacío.
 */
int btree_empty(BTree nodo) { return nodo == NULL; }

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right) {
  BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->data = dato;
  nuevoNodo->left = left;
  nuevoNodo->right = right;
  return nuevoNodo;
}

/**
 * Recorrido del arbol, utilizando la funcion pasada.
 */
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden, FuncionVisitante visit) {
  if (!btree_empty(arbol)) {
    if (orden == BTREE_RECORRIDO_PRE) {
      visit(arbol->data);
      btree_recorrer(arbol->left, orden, visit);
      btree_recorrer(arbol->right, orden, visit);
    }
    else {
      if (orden == BTREE_RECORRIDO_POST) {
        btree_recorrer(arbol->left, orden, visit);
        btree_recorrer(arbol->right, orden, visit);
        visit(arbol->data);
      }
      else  {
        btree_recorrer(arbol->left, orden, visit);
        visit(arbol->data);
        btree_recorrer(arbol->right, orden, visit);
      }
    }
  }
}

int btree_nnodes(BTree tree) {
  int No = 0;
  if (!btree_empty(tree)) {
    No++;
    No += btree_nnodes(tree->right);
    No += btree_nnodes(tree->left);
  }
  return No;
}

int btree_search(BTree tree, int num) {
  if (!btree_empty(tree))
    return tree->data == num || btree_search(tree->left, num) || btree_search(tree->right, num);
  return 0;
}

BTree btree_copy(BTree tree) {
  BTree new_tree = btree_crear();
  if(!btree_empty(tree)) {
    new_tree = btree_unir(tree->data, tree->left, tree->right);
  }

  return new_tree;
}

int btree_height(BTree tree) {
  int h = 0;
}
