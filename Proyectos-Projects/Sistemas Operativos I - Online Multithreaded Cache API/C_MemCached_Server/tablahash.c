#include "tablahash.h"

unsigned DJB2Hash(char *s, size_t len) {
  unsigned hashval = 0;
  for (unsigned long i = 0; i < len; i++) {
    hashval = ((hashval << 5) + hashval) + *s;  /* hash * 33 + s */
  }
  return hashval;
}

TablaHash tablahash_crear(size_t capacidad, int locks) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(DNode) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->cantLocks = locks;
  tabla->locks = malloc(sizeof(pthread_mutex_t) * locks);
  for (int i = 0; i < locks; i++) {
    pthread_mutex_init(&(tabla->locks[i]), NULL);
  }
  tabla->nElemsLock = malloc(sizeof(pthread_spinlock_t));
	pthread_spin_init(tabla->nElemsLock, 0);

  // Creamos cola de historial de la tablaHash
  tabla->historial.newest = NULL;
  tabla->historial.oldest = NULL;
  pthread_mutex_init(&(tabla->historial.historyMutex), NULL);

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx] = NULL;
  }

  return tabla;
}

void tablahash_destruir(TablaHash tabla) {
  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx] != NULL)
      dlist_destruir(tabla->elems[idx]);

  for(int i = 0; i < tabla->cantLocks; i++) {
    pthread_mutex_destroy(&(tabla->locks[i]));
  }
  pthread_mutex_destroy(&(tabla->historial.historyMutex));
  pthread_spin_destroy(tabla->nElemsLock);
  free((void *) tabla->nElemsLock);
  free(tabla->locks);
  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

int tablahash_insertar(TablaHash *tabla, char* clave, char* valor, size_t lenClave, size_t lenValor, bool bin) {
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = DJB2Hash(clave, lenClave) % (*tabla)->capacidad;
  int lock = idx / ((*tabla)->capacidad / (*tabla)->cantLocks);

  pthread_mutex_lock(&(*tabla)->locks[lock]);
  // Localizar el elemento, si está

  
  DList elem = dlist_buscar_y_retornar((*tabla)->elems[idx], clave, lenClave);
  if (elem == NULL) { // No hay valor asociado a la clave en la tabla hash+
    // Insertar el dato.
    pthread_spin_lock((*tabla)->nElemsLock);
    (*tabla)->numElems++;
    pthread_spin_unlock((*tabla)->nElemsLock);
    DNode* nuevo;
    bool sueltalock = false;
    // Se eliminan las keys menos útiles hasta tener espacio suficiente
    while((nuevo = dlist_agregar_inicio((*tabla)->elems[idx], clave, lenClave, valor, lenValor, bin)) == NULL) {
      if ((*tabla)->historial.newest == NULL) {         // Eliminamos todos los elementos 
        pthread_mutex_unlock(&(*tabla)->locks[lock]);   // de la tabla y aún así no hay espacio
        return 2; 
      }
     
      pthread_mutex_unlock(&(*tabla)->locks[lock]);
      eliminar_menos_util(tabla);
      pthread_mutex_lock(&(*tabla)->locks[lock]);
      sueltalock = true;
    }
    if (sueltalock) {
      // Nos cercioramos de que el nodo no fue creado 
      // mientras se eliminaba el menos útil de la tabla
      DList elem = dlist_buscar_y_retornar((*tabla)->elems[idx], clave, lenClave);
      if (elem == NULL) {
        (*tabla)->elems[idx] = nuevo;
        hq_insertar(&((*tabla)->historial), nuevo); // Es el elemento "más útil"
        pthread_mutex_unlock(&(*tabla)->locks[lock]);
        return 1;
      }
      // No es correcto insertar el nodo.
      else dnode_destruir(nuevo);
    }
    else {
      (*tabla)->elems[idx] = nuevo;
      hq_insertar(&((*tabla)->historial), nuevo); // Es el elemento "más útil"
      pthread_mutex_unlock(&(*tabla)->locks[lock]);
      return 1;
    }
  }
  // Pisar el valor anterior
  char* valorAntiguo = elem->valor;
  elem->binario = bin;
  if (bin) { // La memoria fue alocada en handle_bin
    elem->valor = valor;
  } else {
    while((elem->valor = malloc(sizeof(char)*lenValor)) == NULL) {
      if ((*tabla)->historial.newest == NULL) {         // Eliminamos todos los elementos 
        pthread_mutex_unlock(&(*tabla)->locks[lock]);   // de la tabla y aún así no hay espacio
        return 2; 
      }
      pthread_mutex_unlock(&(*tabla)->locks[lock]);
      eliminar_menos_util(tabla);
      pthread_mutex_lock(&(*tabla)->locks[lock]);
    }
    strncpy(elem->valor, valor, lenValor);
  }
  elem->lenValor = lenValor;
  free(valorAntiguo);
  // Actualizar la utilidad de la clave
  hq_update(&((*tabla)->historial), elem);
  pthread_mutex_unlock(&(*tabla)->locks[lock]);
  return 0;
}

char* tablahash_buscar_y_retornar(TablaHash *tabla, char *clave, size_t lenClave, size_t *lenValor, bool* isBinary) {
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = DJB2Hash(clave, lenClave) % (*tabla)->capacidad;
  int lock = idx / ((*tabla)->capacidad / (*tabla)->cantLocks);
  pthread_mutex_lock(&(*tabla)->locks[lock]);
  DList elem =
      dlist_buscar_y_retornar((*tabla)->elems[idx], clave, lenClave);
  if (elem != NULL) {
    // Determinamos si el valor se ingresó en binario
    *isBinary = elem->binario;
    char* valor = elem->valor;
    *lenValor = elem->lenValor;
    hq_update(&((*tabla)->historial), elem);
    pthread_mutex_unlock(&(*tabla)->locks[lock]);
    return valor;
  }
  pthread_mutex_unlock(&(*tabla)->locks[lock]);
  // Si no se encontró la clave
  *isBinary = false;
  return NULL;
}

bool tablahash_eliminar(TablaHash *tabla, char *clave, size_t lenClave) {
  unsigned idx = DJB2Hash(clave, lenClave) % (*tabla)->capacidad;
  int lock = idx / ((*tabla)->capacidad / (*tabla)->cantLocks);
  pthread_mutex_lock(&(*tabla)->locks[lock]);
  DList elem =
      dlist_buscar_y_retornar((*tabla)->elems[idx], clave, lenClave);
  bool found;
  
  if (elem != NULL) {
    pthread_spin_lock((*tabla)->nElemsLock);
    (*tabla)->numElems--;
    pthread_spin_unlock((*tabla)->nElemsLock);
    // Se retira el nodo de la cola historial y la tablahash
    pthread_mutex_lock(&((*tabla)->historial.historyMutex));
    hq_eliminar(&((*tabla)->historial), &((*tabla)->elems[idx]), elem);
    pthread_mutex_unlock(&((*tabla)->historial.historyMutex));
    found = true;
  } else found = false;

  pthread_mutex_unlock(&(*tabla)->locks[lock]);
  return found; // Si es false, no se encontró, (ENOTFOUND)
}

void eliminar_menos_util(TablaHash *tabla) {
  // Localizar el elemento menos recientemente usado
  DNode* aEliminar = (*tabla)->historial.oldest;
  unsigned idx = DJB2Hash(aEliminar->clave, aEliminar->lenClave) % (*tabla)->capacidad;
  int lock = idx / ((*tabla)->capacidad / (*tabla)->cantLocks);
  // Los mutexes son tomados sólo unavez se haya encontrado el elemento, pues sino puede
  // producirse deadlocks
  pthread_mutex_lock(&(*tabla)->locks[lock]);
  pthread_mutex_lock(&(*tabla)->historial.historyMutex);
  if (aEliminar == (*tabla)->historial.oldest) {            // Si el elemento sigue en la tabla
    hq_eliminar(&((*tabla)->historial), &((*tabla)->elems[idx]), aEliminar);
    pthread_spin_lock((*tabla)->nElemsLock);
    (*tabla)->numElems--;
    pthread_spin_unlock((*tabla)->nElemsLock);
  }
  // En caso que el elemento más antiguo haya cambiado durante la ejecución de la función,
  // no se eliminará nada. Sin embargo, la función deberá llamarse siempre utilizando un loop
  // while que eliminará elementos hasta que haya suficiente memoria.
  pthread_mutex_unlock(&(*tabla)->historial.historyMutex);
  pthread_mutex_unlock(&(*tabla)->locks[lock]);
}

void *increase_size(void *dir, size_t newSize, TablaHash *tabla) {
	if (newSize > MEMORY_LIM)
		return NULL;
	void *nuevo;
	while((nuevo = realloc(dir, newSize)) == NULL) {
		if((*tabla)->historial.newest == NULL)
			return NULL;
		eliminar_menos_util(tabla);
	}
	return nuevo;
}