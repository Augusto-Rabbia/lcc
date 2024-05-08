#include "btree.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>


typedef _Pila {
    void **arr;
    int top;
} Stack;

Stack pila_crear(int size) {
    Stack pila;
    pila.arr = malloc((sizeof pila.arr[0]) * size);
    pila.top = -1;
}

/*void pila_agrandar(Stack* pila, int newsize) {
    pila.arr =
}*/

void pila_apilar(Stack* pila, void* dato) {
    pila->top++;
    pila->arr[pila->top] = dato;
}

void pila_desapilar(Stack* pila) {
    pila->top--;
}

void* pila_tope(Stack pila) {
    return pila.arr[pila.top];
}



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
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,
                    FuncionVisitante visit) {

  if (!btree_empty(arbol)) {
    if(orden == BTREE_RECORRIDO_IN) {
        btree_recorrer(arbol->left, orden, visit);
        visit(arbol->data);
        btree_recorrer(arbol->right, orden, visit);
    }
    if(orden == BTREE_RECORRIDO_PRE) {
        visit(arbol->data);
        btree_recorrer(arbol->left, orden, visit);
        btree_recorrer(arbol->right, orden, visit);
    }
    if(orden == BTREE_RECORRIDO_POST) {
        btree_recorrer(arbol->left, orden, visit);
        btree_recorrer(arbol->right, orden, visit);
        visit(arbol->data);
    }
  }
}

void btree_recorrer_preorden(BTree arbol) {
    Stack s = pila_crear(2000);
    pila_apilar(&s, arbol);
    while (s->top >= 0) {
        BTree n = pila_tope(s);
        pila_desapilar(&s);
        if (!n)
          continue;

        printf("%d ", n->data);
        pila_apilar(&s, n->right);
        pila_apilar(&s, n->left);
    }
}

void btree_recorrer_inorden(BTree arbol) {
    Stack s = pila_crear(3000);
    pila_apilar(&s, arbol);
    while (s->top >= 0) {
        BTree n = tope(s);
        desapilar(&s);

        if (!n)
          continue;
        pila_apilar(&s, n->right);
        pila_apilar(&s, n);
        n = n->left;
        printf("%d ", n->data);
    }
}



int max2(int n, int m) {
    if(n>m)
        return n;
    return m;
}

int btree_altura(BTree tree) {
    if (!btree_empty(tree)) {
        return( max2(btree_altura(tree->left), btree_altura(tree->right)) + 1);
    }
    return 0;
}

int btree_n_nodos_profundidad(BTree tree, int depth) {
    if (!btree_empty(tree)) {
        if (depth != 1) {
            return btree_n_nodos_profundidad(tree->left, depth-1) + btree_n_nodos_profundidad(tree->right, depth-1);
        }
        else {
            return 1;
        }
    }
    return 0;
}

int btree_profundidad_nodo(BTree tree, int dato) {
    if (!btree_empty(tree)) {
        if (tree->data == dato) {
            return 1;
        }
        if(btree_profundidad_nodo(tree->left, dato) >= 1) {
            return btree_profundidad_nodo(tree->left, dato) + 1;
        }
        if(btree_profundidad_nodo(tree->right, dato) >= 1) {
            return btree_profundidad_nodo(tree->right, dato) + 1;
        }
    }
    return -1;
}

void btree_recorrer_god(BTree arbol, BTreeOrdenDeRecorrido orden, FuncionVisitanteGod visit, void* extra) {
    if (!btree_empty(arbol)) {
      if(orden == BTREE_RECORRIDO_IN) {
          btree_recorrer_god(arbol->left, orden, visit, extra);
          visit(arbol->data, extra);
          btree_recorrer_god(arbol->right, orden, visit, extra);
      }
      else if(orden == BTREE_RECORRIDO_PRE) {
          visit(arbol->data, extra);
          btree_recorrer_god(arbol->left, orden, visit, extra);
          btree_recorrer_god(arbol->right, orden, visit, extra);
      }
      else {
          btree_recorrer_god(arbol->left, orden, visit, extra);
          btree_recorrer_god(arbol->right, orden, visit, extra);
          visit(arbol->data, extra);
      }
    }
}




void btree_recorrer_por_ext(BTree tree, FuncionVisitante visit) {
    if (!btree_empty(tree)) {

        btree_recorrer_por_ext(tree->left, visit);
        btree_recorrer_por_ext(tree->right, visit);

    }
}
