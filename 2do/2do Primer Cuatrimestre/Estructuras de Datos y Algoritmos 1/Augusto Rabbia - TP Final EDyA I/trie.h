#ifndef __TRIE_H__
#define __TRIE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "slist.h"

#define NUM_CHARS 26

typedef struct _TrieNode {
    struct _TrieNode *children[NUM_CHARS];
    bool terminal;
    SList encontradas;
}* Trie;

/**
 * Crea un trie con todos sus hijos = NULL y terminal = false. 
 */
Trie trie_crear();

/**
 * Destruye un trie. 
 */
void trie_destruir(Trie root);

/**
 * Inserta un nuevo string en un trie. 
 */
void trie_insertar(Trie* root, unsigned char* str);

/**
 * Devuelve true si el string está en el trie. false en caso contrario.
 */
bool trie_buscar(Trie trie, unsigned char* str);

#endif