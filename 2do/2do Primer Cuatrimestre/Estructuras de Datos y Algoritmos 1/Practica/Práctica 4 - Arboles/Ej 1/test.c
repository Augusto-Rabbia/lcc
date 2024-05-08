#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

static void imprimir_entero(int data) {
  printf("%d ", data);
}

int main() {
  BTree lrrr = btree_unir(9, btree_crear(), btree_crear());
  BTree lrrl = btree_unir(8, btree_crear(), btree_crear());
  BTree lrr = btree_unir(7, lrrl, lrrr);
  BTree ll = btree_unir(1, btree_crear(), btree_crear());
  BTree lr = btree_unir(5, btree_crear(), lrr);
  BTree l = btree_unir(2, ll, lr);
  BTree rr = btree_unir(6, btree_crear(), btree_crear());
  BTree r = btree_unir(3, btree_crear(), rr);
  BTree raiz = btree_unir(4, l, r);

  btree_recorrer(raiz, BTREE_RECORRIDO_PRE, imprimir_entero);
  puts("");
  btree_recorrer(raiz, BTREE_RECORRIDO_IN, imprimir_entero);
  puts("");
  btree_recorrer(raiz, BTREE_RECORRIDO_POST, imprimir_entero);
  puts("");

  printf("\nCant Nodos: %d", btree_nnodes(raiz));
  printf("\n5 in tree?: %d\n", btree_search(raiz, 5));

  BTree raiz2 = btree_copy(raiz);
  btree_recorrer(raiz2, BTREE_RECORRIDO_PRE, imprimir_entero);
  puts("");

  btree_destruir(raiz);

  return 0;
}
