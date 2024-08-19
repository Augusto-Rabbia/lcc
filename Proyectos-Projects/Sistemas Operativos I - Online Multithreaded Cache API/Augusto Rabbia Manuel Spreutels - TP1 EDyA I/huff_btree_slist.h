#ifndef __HUFF_BTREE_SLIST_H__
#define __HUFF_BTREE_SLIST_H__

typedef struct _BTNode {
    int data;
    struct _BTNode* right;
    struct _BTNode* left;
} *BTree;

typedef struct _WTnode {
  struct _BTNode* arbol;
  int peso;
} WTnode;

typedef WTnode *WeightedTree;

typedef struct _SNodo {
  WeightedTree wtree;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

/*     SLIST     */

/**
 * Devuelve una lista vacía.
 */
SList slist_crear();

/**
 * Agrega un elemento al final de la lista.
 */
SList slist_agregar_final(SList lista, WeightedTree warbol);

/**
 * Agrega un elemento al inicio de la lista.
 */
SList slist_agregar_inicio(SList lista, WeightedTree warbol);

/**
 * Inserta un arbol ordenado según su peso.
 */
SList slist_insertar_ordenado(SList lista, WeightedTree wtree);

/*     BTREE     */

/**
 * Devuelve un arbol vacío (con valor NULL).
 */
BTree btree_crear();

/**
 * Crea un arbol con con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_conectar(int data, BTree r, BTree l);

/**
 * Libera la memoria dedicada a un arbol, recorriéndolo.
 */
void btree_destruir(BTree tree);

#endif
