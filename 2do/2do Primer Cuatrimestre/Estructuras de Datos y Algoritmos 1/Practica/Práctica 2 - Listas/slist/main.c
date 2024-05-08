#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

static void imprimir_entero(int dato) {
  printf("%d ", dato);
}

int main(int argc, char *argv[]) 
{
  SList L1 = slist_crear();
  L1 = slist_agregar_inicio(L1, 3);
  L1 = slist_agregar_inicio(L1, 2);
  L1 = slist_agregar_inicio(L1, 1);
  L1 = slist_agregar_final(L1, 4);

  slist_recorrer(L1, imprimir_entero);
  puts("");

  printf("L1 len = %d\n\n", slist_len(L1));

  SList L2 = NULL;
  L2 = slist_agregar_inicio(L2, 7);
  L2 = slist_agregar_inicio(L2, 6);
  
  slist_concat(&L1, L2);
  slist_recorrer(L1, imprimir_entero);
  puts("");

  slist_insert(&L1, 4, 5);
  slist_recorrer(L1, imprimir_entero);
  puts("");

  slist_del(&L1, 6);
  //slist_del(&L1, 0); se puede por el & sino no se podría
  slist_recorrer(L1, imprimir_entero);
  printf("\nis 5 in L1? : %d\n", slist_contains(L1, 5));

  SList L3 = NULL;
  L3 = slist_agregar_inicio(L3, 7);
  L3 = slist_agregar_inicio(L3, 6);
  L3 = slist_agregar_inicio(L3, 5);
  L3 = slist_agregar_inicio(L3, 4);
  slist_recorrer(slist_intersect(L1, L3), imprimir_entero);

  //puts("\ninverso:");
  SList L4 = slist_crear();
  L4 = slist_agregar_inicio(L4, 3);
  L4 = slist_agregar_inicio(L4, 2);
  L4 = slist_agregar_inicio(L4, 1);
  L4 = slist_agregar_final(L4, 4);
  //SList invL1 = slist_reverso(L1);
  //slist_recorrer(invL1, imprimir_entero);

  puts("");
  slist_recorrer(slist_intercalar(L1,L3), imprimir_entero);

  

  slist_destruir(L1);
  slist_destruir(L2);
  slist_destruir(L3);
  slist_destruir(L4);

  return 0;
}
