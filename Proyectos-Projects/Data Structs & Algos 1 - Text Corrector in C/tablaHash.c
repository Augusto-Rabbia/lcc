#include "tablaHash.h"

unsigned long DJB2Hash(char *s) {
    unsigned long hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = ((hashval << 5) + hashval) + *s; /* hash * 33 + c */
    }
    return hashval;
}

tablaHash tablahash_crear(unsigned size, FunHash hash) {
    // Pedimos memoria para la estructura principal y las casillas.
    tablaHash tabla = malloc(sizeof(struct _TablaHash));
    assert(tabla != NULL);
    tabla->elems = malloc(sizeof(SList) * size);
    assert(tabla->elems != NULL);
    tabla->numElems = 0;
    tabla->size = size;
    tabla->hash = hash;

    // Inicializamos las casillas con datos nulos.
    for (unsigned idx = 0; idx < size; idx++) {
        tabla->elems[idx] = NULL;
    }
    return tabla;
}

void tablahash_destruir(tablaHash tabla) {
    for (unsigned i = 0; i < tabla->size; i++) {
        if (tabla->elems[i] != NULL) {
            free(tabla->elems[i]->str);
            free(tabla->elems[i]);
        }
    }
    // Liberar el arreglo de casillas y la tabla.
    free(tabla->elems);
    free(tabla);
    return;
}

void tablahash_vaciar(tablaHash tabla) {
    for (unsigned int i = 0; i < tabla->size; i++) {
        if (tabla->elems[i] != NULL) {
            free(tabla->elems[i]->str);
            free(tabla->elems[i]);
            tabla->elems[i] = NULL;
        }
    }
}

unsigned tablahash_encontrar_espacio(tablaHash tabla, char* dato, unsigned idxInicio) {
    for(unsigned i = idxInicio+1; i != idxInicio; i++) {
        if (i == tabla->size)
            i = 0;
        if(tabla->elems[i] == NULL) 
            return i;

        // Si la palabra ya está en la tabla, devolvemos el index inicial 
        //(consideramos que no se va a borrar ninguna palabra en la tabla).
        if(strcmp(tabla->elems[i]->str, dato) == 0)
            return idxInicio;
    }
    // No encontró ningún espacio (no debería poder ocurrir).
    return idxInicio;
}

void tablahash_insertar(tablaHash* tabla, char *dato, int idxMaxMod, int pasosRestantes) {
    if (((*tabla)->numElems / (double) (*tabla)->size) > MAX_FACTOR_DE_CARGA) {
        *tabla = tablahash_redimensionar(*tabla);
    }

    // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned long hash = (*tabla)->hash(dato);
    unsigned idx = hash % (*tabla)->size;

    // Si la casilla está libre, creamos una lista con la palabra.
    if ((*tabla)->elems[idx] == NULL) {
        
        (*tabla)->numElems++;
        (*tabla)->elems[idx] = slist_agregar_final(NULL, dato, idxMaxMod, pasosRestantes);
    }
    // Sino, encontramos la próxima casilla libre, y la linkeamos con la anterior
    // con una lista simplemente enlazada.
    else {
        unsigned idxNew = tablahash_encontrar_espacio((*tabla), dato, idx);
        if (idxNew != idx) {
            SNode* newNode = slist_agregar_final(NULL, dato, idxMaxMod, pasosRestantes);
            (*tabla)->elems[idxNew] = newNode;
            slist_agregar_nodo_final((*tabla)->elems[idx], newNode);
            (*tabla)->numElems++;
        }
    }
}

tablaHash tablahash_redimensionar(tablaHash tabla) {
    tablaHash newTable = tablahash_crear((tabla->size * 2), tabla->hash);
    for (unsigned i = 0; i < tabla->size; i++) {
        if (tabla->elems[i] != NULL) {
            tablahash_insertar(&newTable, tabla->elems[i]->str, tabla->elems[i]->idxGuardado, tabla->elems[i]->pasosRestantes);
        }
    }
    tablahash_destruir(tabla);
    return newTable;
}

int tablaHash_buscar_index(tablaHash tabla, char* str) {
    // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(str) % tabla->size;
    // Retornar -1 si la casilla estaba vacia.
    if (tabla->elems[idx] == NULL)
        return -1;
    // Si el elemento está en la tabla, retorna el index que tenga guardado.
    // Este index será idxEncontrada, que indica cuál es la posición de la
    // palabra en el array de palabras incorrectas.
    else 
        for(SList temp = tabla->elems[idx]; temp != NULL; temp = temp->next) {
            if (strcmp(temp->str, str) == 0)
                return temp->idxGuardado;
        }
    //Sino, retornar -1
    return -1;
}

int tablaHash_buscar_filtrar_repetidas(tablaHash tabla, char* palabraStr, int* idxMaxMod, int pasosRestantes) {
    // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(palabraStr) % tabla->size;
    // Devolver 0 si la casilla estaba vacia.
    if (tabla->elems[idx] == NULL)
        return 0;
    // Verificamos si el elemento está en algún lugar en la lista.
    else 
        for(SList temp = tabla->elems[idx]; temp != NULL; temp = temp->next) {
            if (strcmp(temp->str, palabraStr) == 0) {
                // Si encontramos el elemento, verificamos si se encontró en menos pasos que la última vez.
                if (temp->pasosRestantes < pasosRestantes) {
                    // buscarAdyacente representa si vamos o no a buscar palabras adyacentes sobre esta palabra
                    // 3 representa que debemos buscarlas porque la última vez que encontramos esta palabra
                    // no nos quedaban más pasos y no pudimos, sin embargo, en este encuentro, sí nos quedan.
                    // 2 representará que esto no es necesario porque ya buscamos en la anterior ocasión.
                    int buscarAdyacente; 
                    if (temp->pasosRestantes == 1) 
                        buscarAdyacente = 3;
                    else buscarAdyacente = 2;
                    temp->pasosRestantes = pasosRestantes;
                    *idxMaxMod = tabla->elems[idx]->idxGuardado;
                    return buscarAdyacente;
                }
                return 1;
            }
        }
    //Sino, devolver 0
    return 0;
}
