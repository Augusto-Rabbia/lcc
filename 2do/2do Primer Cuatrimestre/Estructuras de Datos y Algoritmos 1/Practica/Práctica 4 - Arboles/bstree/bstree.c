#include "bstree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Estructura del nodo del arbol de busqueda binaria.
 * Tiene un puntero al dato (dato),
 * un puntero al nodo raiz del subarbol izquierdo (izq),
 * y un puntero al nodo raiz del subarbol derecho (der).
 */
struct _BST_Nodo {
  void *dato;
  struct _BST_Nodo *izq, *der;
};

/**
 * bstee_crear: Retorna un arbol de busqueda binaria vacio
 */
BSTree bstee_crear() { return NULL; }

/**
 * bstree_destruir: Destruye el arbol y sus datos
 */
void bstree_destruir(BSTree raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    bstree_destruir(raiz->izq, destr);
    bstree_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
};

/**
 * bstree_buscar: Retorna 1 si el dato se encuentra y 0 en caso
 * contrario
 */
int bstree_buscar(BSTree raiz, void *dato, FuncionComparadora comp) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return bstree_buscar(raiz->izq, dato, comp);
  else // raiz->dato < dato
    return bstree_buscar(raiz->der, dato, comp);
}

/**
 * bstree_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad del arbol de busqueda binaria
 */
BSTree bstree_insertar(BSTree raiz, void *dato, FuncionCopiadora copia,
                       FuncionComparadora comp) {
  if (raiz == NULL) { // insertar el dato en un nuevo nodo
    struct _BST_Nodo *nuevoNodo = malloc(sizeof(struct _BST_Nodo));
    assert(nuevoNodo != NULL);
    nuevoNodo->dato = copia(dato);
    nuevoNodo->izq = nuevoNodo->der = NULL;
    return nuevoNodo;
  } else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    raiz->izq = bstree_insertar(raiz->izq, dato, copia, comp);
  else if (comp(dato, raiz->dato) > 0) // raiz->dato < dato
    raiz->der = bstree_insertar(raiz->der, dato, copia, comp);
  // si el dato ya se encontraba, no es insertado
  return raiz;
}

/**
 * bstree_recorrer: Recorrido DSF del arbol
 */
void bstree_recorrer(BSTree raiz, BSTreeRecorrido orden,
                     FuncionVisitanteExtra visita, void *extra) {
  if (raiz != NULL) {
    if (orden == BTREE_RECORRIDO_PRE)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->izq, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_IN)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->der, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}

BSTree bstree_max(BSTree tree) {
    if (tree->der->der == NULL || tree->der == NULL) {
        return tree;
    }
    return bstree_max(tree->der);
}

BSTree bstree_max_dato(BSTree tree) {
    if (tree->der == NULL) {
        return tree->dato;
    }
    return bstree_max(tree->der);
}

BSTree bstree_del(BSTree root, void* dato, FuncionComparadora comp, FuncionDestructora destr) {
    if(root == NULL) return NULL;
    if (comp(root->dato, dato) == 0) {
        if(root->izq == NULL) {
            BSTree temp = root->der;
            destr(root);
            return temp;
        }
        BSTree temp = bstree_max(root->izq);
        if(temp->der == NULL) {
            temp->der = root->der;
            destr(root);
            return temp;
        }
        else {
            BSTree hijos = temp->der->izq;
            BSTree temp2 = temp->der;
            temp2->der = root->der;
            temp2->izq = root->izq;
            destr(root);
            temp->der = hijos;
            return temp2;
        }
    }
    if(comp(root->dato, dato) < 0) {
        root->der = bstree_del(root->der, dato, comp, destr);
    }
    else {
        root->izq = bstree_del(root->izq, dato, comp, destr);
    }
    return root;

}

int bstree_size(BSTree tree) {
    if (tree == NULL) return 0;
    return 1 + bstree_size(tree->der) + bstree_size(tree->izq);
}

BSTree bstree_k_esimo_menor(BSTree tree, int k) {
    if (tree != NULL) {
        int tam_izq = bstree_size(tree->izq);
        printf("k: %d, tam: %d\n", k, tam_izq);
        if (tam_izq >= k)
            return bstree_k_esimo_menor(tree->izq, k);
        if(tam_izq == k -1)
            return tree;
        return bstree_k_esimo_menor(tree->der, k-tam_izq-1);
    }
    return NULL;
}

int BSTree_validate(BSTree tree, FuncionComparadora comp) {


    return comp(tree->dato, tree->izq->dato)
    && BSTree_validate(tree->izq, comp) && BSTree_validate(tree->izq, comp)
}
