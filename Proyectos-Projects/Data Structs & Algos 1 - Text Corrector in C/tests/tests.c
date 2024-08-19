#include <stdio.h>
#include "../trie.h"
#include "../corrector.h"

Palabra test_palabras_incorrectas(Trie dict) {
    int cantIncorrectas = 0;
    Palabra* pI = palabras_incorrectas("test.txt", &cantIncorrectas, dict);
    Palabra p = pI[0];
    assert(strcmp(p->str, "zzz"));
    assert(p->lineas[0] == 1);
    assert(p->cantLineas == 1);
    assert(p->cantSugerencias == 0);
    strcpy(p->str, "vivimos");

    free(pI);
    return p;
}

void test_palabra_agregar_sugerencia(Palabra p) {
    palabra_agregar_sugerencia(p, "test");
    assert(strcmp(p->sugerencias[0], "test") == 0);
}

void test_buscar_palabras_adyacentes(Trie dict, Palabra p) {
    int idxMaxMod = -MAX_CORRECCIONES;
    buscar_palabras_adyacentes(dict, p, &idxMaxMod, 1, 0);
    assert(idxMaxMod == -MAX_CORRECCIONES);
    assert(p->cantSugerencias == 3);
    assert(strcmp(p->sugerencias[1], "vi vimos") == 0);
    assert(strcmp(p->sugerencias[2], "vivimos") == 0);
}

void test_separar_palabras() {
    char* t1 = separar_palabras("estoes", 4);
    char* t2 = separar_palabras("untest", 2);
    assert(strcmp("esto es", t1) == 0);
    assert(strcmp("un test", t2) == 0);
    free(t1);
    free(t2);
}

void test_intercambiar_modificar_insertar_eliminar_letras() {
    char* t = malloc(sizeof(char) * 256);
    strcpy(t, "test");

    intercambiar_letras(&t, 0);
    assert(strcmp(t, "etst") == 0);
    modificar_letra(&t, 2, 'z');
    assert(strcmp(t, "etzt") == 0);
    insertar_letra(&t, 4, 0, 'x');
    insertar_letra(&t, 5, 5, 'w');
    assert(strcmp(t, "xetztw") == 0);
    eliminar_letra(&t, 6, 0);
    eliminar_letra(&t, 5, 4);
    assert(strcmp(t, "etzt") == 0);
    
    free(t);
}

void test_escribir_archivo(Palabra p) {
    escribir_archivo(&p, 1, "testOutput");
}

void test_tablahash() {
    tablaHash t = tablahash_crear(3, (FunHash) DJB2Hash);
    assert(t != NULL);
    // tablahash_insertar
    tablahash_insertar(&t, "a", 2, 2);
    assert(strcmp(t->elems[DJB2Hash("a") % 3]->str, "a") == 0);
    assert(t->elems[DJB2Hash("a") % 3]->idxGuardado == 2);

    // tablahash_buscar_filtrar_repetidas
    int idx = -1;
    assert(tablaHash_buscar_filtrar_repetidas(t, "a", &idx, 3) == 2);
    assert(tablaHash_buscar_filtrar_repetidas(t, "b", &idx, 3) == 0);
    assert(tablaHash_buscar_filtrar_repetidas(t, "a", &idx, 1) == 1);

}

int main(void) {
    Trie dict = crear_trie_diccionario("testdict.txt");
    Palabra p = test_palabras_incorrectas(dict);

    test_separar_palabras();
    test_palabra_agregar_sugerencia(p);
    test_buscar_palabras_adyacentes(dict, p);
    test_intercambiar_modificar_insertar_eliminar_letras();
    test_escribir_archivo(p);
    test_tablahash();

    trie_destruir(dict);
    palabra_destruir(p);
    printf("Fin de los tests.\n");
    return 0;
}
