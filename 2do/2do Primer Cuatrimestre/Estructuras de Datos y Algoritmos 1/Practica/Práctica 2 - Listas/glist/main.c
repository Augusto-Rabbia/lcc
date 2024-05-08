#include "contacto.h"
#include "glist.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  GList lista = glist_crear();
  Contacto *contactos[6];
  contactos[0] = contacto_crear("Pepe", "3412695452", 16);
  contactos[1] = contacto_crear("Moni", "3412684759", 60);
  contactos[2] = contacto_crear("Juan", "3415694286", 32);
  contactos[3] = contacto_crear("Paola", "3416259862", 29);
  contactos[4] = contacto_crear("Maria", "3416874594", 15);
  contactos[5] = contacto_crear("Jose", "3416894526", 13);

  for (int i = 0; i < 6; ++i) {
    lista =
        glist_agregar_inicio(lista, contactos[i], (FuncionCopia)contacto_copia);
    contacto_destruir(contactos[i]);
  }
  
  GList l2 = glist_crear();
  //l2 = glist_filter(lista, (FuncionCopia)contacto_copia, (Predicado)contact_adult);

  printf("Lista:\n");
  glist_recorrer(lista, (FuncionVisitante)contacto_imprimir);

  glist_destruir(lista, (FuncionDestructora)contacto_destruir);
  //glist_destruir(l2, (FuncionDestructora)contacto_destruir);

  return 0;
}