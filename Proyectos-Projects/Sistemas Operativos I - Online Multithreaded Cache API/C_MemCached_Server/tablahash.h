#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include "dlist.h"
#include "common.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/**
 * Estructura principal que representa la tabla hash concurrente.
 */
struct _TablaHash {
  DList *elems;
  uint64_t numElems;
  pthread_spinlock_t* nElemsLock;
  size_t capacidad;
  pthread_mutex_t* locks;
  int cantLocks;
  HistoryQueue historial;
};

typedef struct _TablaHash *TablaHash;

/**
 * Función hash.
 */
unsigned DJB2Hash(char *s, size_t len);

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(size_t capacidad, int locks);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);

/**
 * Devuelve 1 si el elemento insertado no existía en la tablahash, 0 en otro caso.
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * Si el elemento no se pudo insertar por falta de memoria (por haber sido demasiado grande), devuelve 2
 */
int tablahash_insertar(TablaHash *tabla, char* clave, char* valor, size_t lenClave, size_t lenValor, bool bin);

/**
 * Devuelve si se eliminó un elemento o no.
 */
bool tablahash_eliminar(TablaHash *tabla, char* clave, size_t lenClave);

/**
 * Busca una key en la TablaHash y devuelve su valor asociado. También modifica el puntero a lenValor e isBinary
 * con la información del valor.  
*/
char* tablahash_buscar_y_retornar(TablaHash *tabla, char *clave, size_t lenClave, size_t *lenValor, bool* isBinary);

/**
 * Elimina el elemento más antiguo (según la HistoryQueue) de la tabla. Utilizado para limpiar la memoria
 * si esta se llenara.
*/
void eliminar_menos_util(TablaHash *tabla);

// Reallocates the memory allocated at dir.
// As much memory as needed is freed from the hash table's history queue.
// Returns a pointer to the new location of the allocated memory.
// If the operation fails, NULL is returned.
void *increase_size(void *dir, size_t newSize, TablaHash* tabla);

#endif  /* __TABLAHASH_H__ */
