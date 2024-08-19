#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slist.h"

#define MAX_WORD_LEN 255
#define MAX_FACTOR_DE_CARGA 0.7

typedef unsigned long (*FunHash) (char* word);

/**
 * Funcion Hash.
 */
unsigned long DJB2Hash(char *s);

struct _TablaHash {
  SList* elems;
  unsigned numElems;
  unsigned size;
  FunHash hash;
}; 
typedef struct _TablaHash* tablaHash;

/**
 * Destruye la tabla.
 */
void tablahash_destruir(tablaHash tabla);

/**
 * Libera todos los elementos de la tabla y los incializa como NULL.
 */
void tablahash_vaciar(tablaHash tabla);

/**
 * Crea una tablaHash.
 */
tablaHash tablahash_crear(unsigned size, FunHash hash);

/**
 * Encuentra el primer lugar vacío en la tablaHash y verifica si el string que
 * ya se encuentra en la misma.
 */
unsigned tablahash_encontrar_espacio(tablaHash tabla, char* dato, unsigned idxViejo);

/**
 * Inserta un dato en la tabla. En caso de colisión, se implementa listas
 * mezcladas.
 * idxGuardado puede representar idxMaxMod o idxEncontrada.
 * IMPORTANTE: Por eficiencia, se asume que nunca se remueve un elemento de la tabla.
 */
void tablahash_insertar(tablaHash* tabla, char *dato, int idxGuardado, int pasosRestantes);

/**
 * Redimensiona una tablaHash, rehasheando sus elementos.
 */
tablaHash tablahash_redimensionar(tablaHash table); 

/**
 * Busca un elemmento de la tablaHash y si lo encuentra,
 * devuelve su idxGuardado. En caso contrario, devuelve -1
 */
int tablaHash_buscar_index(tablaHash tabla, char* str);

/**
 * Busca una palabra en la tablaHash. Si no la encuentra, devuelve 0.
 * Sino, si la palabra tiene un valor de pasosRestantes mayor al
 * de entrada de la función devuelve 1, significando que esta palabra
 * ya fue encontrada, en menos pasos que la última vez, y por lo tanto
 * es innecesario seguir con el backtracking. En caso contrario, si la 
 * anterior vez que se encontró la palabra no le quedaban pasos, devuelve
 * 3, para representar que es necesario buscar palabras adyacentes. Sino,
 * devuelve 2. 
 * Adicionalmente, recupera el idxMaxMod guardado al insertar la palabra.
 */
int tablaHash_buscar_filtrar_repetidas(tablaHash tabla, char* palabraStr, int* idxMaxMod, int pasosRestantes);

#endif