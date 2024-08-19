#include <stdio.h>
#include <time.h>
#include "trie.h"
#include "corrector.h"

// MAX_CORRECCIONES y CANT_MAX_SUGERENCIAS están definidas en "palabra.h".
#define DICTIONARY_FILENAME "diccionario.txt"

/**
 * Verifica que el input dado sea correcto.
 * Deben haber 2 argumentos.
 */
void check_input(int argc) {
    if (argc != 3) {
        printf("Cantidad de argumentos inválida. Cerrando programa...\n");
        exit(1);
    }
}



int main(int argc, char* argv[]) {
    check_input(argc);
    Trie trieDiccionario = crear_trie_diccionario(DICTIONARY_FILENAME);
    int cantIncorrectas;
    Palabra* palabrasIncorrectas = palabras_incorrectas(argv[1], &cantIncorrectas, trieDiccionario);

    corregir_palabras(trieDiccionario, palabrasIncorrectas, cantIncorrectas);

    trie_destruir(trieDiccionario);
    
    escribir_archivo(palabrasIncorrectas, cantIncorrectas, argv[2]);

    for (int i = 0; i < cantIncorrectas; i++) {
        palabra_destruir(palabrasIncorrectas[i]);
    }
    free(palabrasIncorrectas);
    return 0;
}