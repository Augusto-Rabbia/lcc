#include "dlist.h"

/**
 * Destruccion de la lista.
 * destroy es una función que libera el dato almacenado.
 */
void dlist_destruir(DList list) {
  DNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;

    free(nodeToDelete->clave);
    free(nodeToDelete->valor);
    free(nodeToDelete);
  }
}

// Destruye un nodo. 
// NO se encarga de modificar sus variables relacionadas a la hq.
void dnode_destruir(DNode* nodo) {
  free(nodo->valor);
  free(nodo->clave);
  free(nodo);
  return;
}

/**
 * Agrega un elemento al inicio de la lista.
 * copy es una función que retorna una copia física del dato.
 */
DList dlist_agregar_inicio(DList list, char* clave, size_t lenC, char *valor, size_t lenVal, bool bin) {
  DNode *newNode = malloc(sizeof(DNode));
  if (newNode == NULL) return NULL;
  newNode->prev = NULL;
  newNode->next = list;
  newNode->binario = bin;
  newNode->lenValor = lenVal;
  newNode->lenClave = lenC;
  // Insertando el valor y clave
  if (bin) {  // Si es binario, entonces el valor y la clave ya están en el heap
    newNode->valor = valor;
    newNode->clave = clave;
  }
  else {
    char* valData = malloc(sizeof(char)*lenVal);
    if (valData == NULL) {
      free(newNode);
      return NULL;
    }
    memcpy(valData, valor, lenVal);
    newNode->valor = valData;
    // Insertando la clave
    char* cData = malloc(sizeof(char)*lenC);
    if (cData == NULL) {
      free(newNode);
      free(valData);
      return NULL;
    }
    memcpy(cData, clave, lenC);
    newNode->clave = cData;
  }

  if(list != NULL)
    list->prev = newNode;
  return newNode;
}



/**
 * Retorna el dato de la lista que coincida con el dato buscado, 
 * o NULL si el dato buscado no se encuentra en la tabla.
*/
DList dlist_buscar_y_retornar(DList list, char* clave, size_t lenC) {
  for (DNode *node = list; node != NULL; node = node->next) {
    if (memcmp(node->clave, clave, lenC) == 0) {
      return node;
    }
  }
  return NULL;
}

// HistoryQueue
void hq_insertar(HistoryQueue *hq, DNode* node) {
  pthread_mutex_lock(&((*hq).historyMutex));
  if ((*hq).newest == NULL) { // La cola está vacía
    node->newer = NULL;
    node->older = NULL;
    (*hq).newest = node;
    (*hq).oldest = node;
  } else {  // El elemento es el más nuevo de la lista
    node->older = (*hq).newest;
    node->newer = NULL;
    (*hq).newest->newer = node;
    (*hq).newest = node;
  }
  
  pthread_mutex_unlock(&((*hq).historyMutex));
}

// Elimina un valor de la hq. No toma el lock de hq, y debe ser tomado antes de llamar a la función.+
// También debe haber sido tomado el lock del elemento de la lista a eliminar.
void hq_eliminar(HistoryQueue *hq, DList* list, DNode* node) {
  free(node->clave);
  free(node->valor);
  if(node->prev != NULL) // No es el primer elemento 
    node->prev->next = node->next;
  // Es el primer elemento y se debe cambiar el pointer de la lista de la respectiva casilla de la tabla hash
  else *list = node->next;

  if(node->next != NULL)
    node->next->prev = node->prev;
    
  if ((*hq).newest == (*hq).oldest) { // Es el único elemento de la cola
    (*hq).newest = NULL;
    (*hq).oldest = NULL;
  } else {
    if ((*hq).newest == node) {     // Cambia el primero
      (*hq).newest = node->older;
      (*hq).newest->newer = NULL;
    } else if ((*hq).oldest == node) { // Cambia el último
      (*hq).oldest = node->newer;
      (*hq).oldest->older = NULL;
    } else {                     // Se conectan sucesor y predecesor
      node->older->newer = node->newer;
      node->newer->older = node->older;
    }
  }
  free(node);
}

void hq_update(HistoryQueue *hq, DNode* node) {
  pthread_mutex_lock(&((*hq).historyMutex));
  // Conectamos los dos nodos adyacentes
  if (node == (*hq).newest) {
    pthread_mutex_unlock(&((*hq).historyMutex));
    return;
  }
  else if (node == (*hq).oldest) { // El sucesor pasa a ser el menos recientemente usado
    (*hq).oldest = node->newer;
    (*hq).oldest->older = NULL;
  } else { // Se "conectan" sucesor y predecesor
    node->older->newer = node->newer;
    node->newer->older = node->older;  
  }
  
  // Cambiamos newest a que sea node
  (*hq).newest->newer = node;
  node->older = (*hq).newest;
  (*hq).newest = node;
  node->newer = NULL;
  
  pthread_mutex_unlock(&((*hq).historyMutex));
}
