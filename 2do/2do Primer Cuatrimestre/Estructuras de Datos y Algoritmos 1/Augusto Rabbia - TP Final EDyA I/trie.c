#include "trie.h"

Trie trie_crear() {
    Trie newTrie = malloc(sizeof(struct _TrieNode));
    for (int i = 0; i < NUM_CHARS; i++) {
        newTrie->children[i] = NULL;
    }
    newTrie->terminal = false;
    
    return newTrie;
}

void trie_destruir(Trie root) {
    if (root != NULL) {
        for (int i = 0; i < NUM_CHARS; i++) {
            trie_destruir(root->children[i]);
        }
        free(root);
    }
}

void trie_insertar(Trie* root, unsigned char* str) {
    if (*root == NULL) {
        *root = trie_crear();
    }
    Trie temp = *root;
    for(int i = 0; str[i] != '\0'; temp = temp->children[str[i]-'a'], i++) {
        if(temp->children[str[i]-'a'] == NULL) {
            temp->children[str[i]-'a'] = trie_crear();
        }
    }
    temp->terminal = true;
}

bool trie_buscar(Trie trie, unsigned char* str) {
    Trie temp = trie;
    for (int i = 0; str[i] != '\0'; i++) {
        if(temp->children[str[i]-'a'] == NULL)
            return false;
        temp = temp->children[str[i]-'a'];
    }
    return temp->terminal;
}
