#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "io.h"
#include "huff_btree_slist.h"
#include "compresor.h"
#include "descompresor.h"

/*   huff_btree_slist.c   */

void test_slist_agregar_inicio(){
    WeightedTree wtree1 = malloc(sizeof(WTnode));
    wtree1->arbol = NULL;
    wtree1->peso = 5;
    WeightedTree wtree2 = malloc(sizeof(WTnode));
    wtree2->arbol = NULL;
    wtree2->peso = 15;
    SList lista = NULL;
    lista = slist_agregar_inicio(lista, wtree2);
    lista = slist_agregar_inicio(lista, wtree1);
    assert(lista->wtree->peso == 5);
    assert(lista->sig->wtree->peso == 15);
    free(wtree1);
    free(wtree2);
    free(lista->sig);
    free(lista);
}

void test_slist_agregar_final() {
    WeightedTree wtree1 = malloc(sizeof(WTnode));
    wtree1->arbol = NULL;
    wtree1->peso = 10;
    SList lista = malloc(sizeof(SNodo));
    lista->sig = NULL;
    lista->wtree = wtree1;

    WeightedTree wtree2 = malloc(sizeof(WTnode));
    wtree2->arbol = NULL;
    wtree2->peso = 15;
    
    lista = slist_agregar_final(lista, wtree2);
    
    assert(lista->sig->wtree->peso == 15);
    free(lista->sig);
    free(lista);
    free(wtree1);
    free(wtree2);
}

void test_slist_insertar_ordenado() {
    WeightedTree wtree1 = malloc(sizeof(WTnode));
    wtree1->arbol = NULL;
    wtree1->peso = 4;
    WeightedTree wtree2 = malloc(sizeof(WTnode));
    wtree2->arbol = NULL;
    wtree2->peso = 3;
    WeightedTree wtree3 = malloc(sizeof(WTnode));
    wtree3->arbol = NULL;
    wtree3->peso = 1;
    WeightedTree wtree4 = malloc(sizeof(WTnode));
    wtree4->arbol = NULL;
    wtree4->peso = 2;
    SList lista = NULL;
    lista = slist_agregar_inicio(lista, wtree2); 
    lista = slist_insertar_ordenado(lista, wtree1);
    lista = slist_insertar_ordenado(lista, wtree3);
    lista = slist_insertar_ordenado(lista, wtree4);

    assert(lista->wtree->peso == 1);
    assert(lista->sig->wtree->peso == 2);
    assert(lista->sig->sig->wtree->peso == 3);
    assert(lista->sig->sig->sig->wtree->peso == 4);
    free(lista->sig->sig->sig);
    free(lista->sig->sig);
    free(lista->sig);
    free(lista);
    free(wtree1);
    free(wtree2);
    free(wtree3);
    free(wtree4);
}

void test_btree_conectar() {
    BTree arbol1 = malloc(sizeof(struct _BTNode));
    BTree arbol2 = malloc(sizeof(struct _BTNode));
    arbol1->left = NULL;
    arbol1->right = NULL;
    arbol1->data = 10;
    arbol2->left = NULL;
    arbol2->right = NULL;
    arbol2->data = 100;
    BTree raiz = btree_conectar(50, arbol1, arbol2);
    assert(raiz->data == 50);
    assert(raiz->right->data == 10);
    assert(raiz->left->data == 100);
    btree_destruir(raiz);
}

/*   compresor.c   */

void test_wtree_crear() {
    WeightedTree wt = wtree_crear(btree_conectar(10, NULL, NULL), 0);
    assert(wt->peso == 0);
    assert(wt->arbol->data == 10);
    free(wt->arbol);
    free(wt);
}

void test_crear_lista_warboles() {
    int *array = calloc(sizeof(int), 256);
    array[0] = 115;
    array[1] = 6;
    array[2] = 200;
    array[3] = 99;
    SList lista = crear_lista_warboles(array);
    SNodo* nodo = lista;
    for(int i = 0; i < 251; nodo = nodo->sig, i++);

    assert(nodo->wtree->peso == 0);
    assert(nodo->sig->wtree->peso == 6);
    assert(nodo->sig->sig->wtree->peso == 99);
    assert(nodo->sig->sig->sig->wtree->peso == 115);
    assert(nodo->sig->sig->sig->sig->wtree->peso == 200);
    free(array);
    nodo = lista;
    for(SList temp; temp != NULL; nodo = temp) {
        free(nodo->wtree->arbol);
        free(nodo->wtree);
        temp = nodo->sig;
        free(nodo);
    }
}

void test_crear_arbol_final() {
    WeightedTree wt6 = wtree_crear(btree_conectar(6, NULL, NULL), 6);
    WeightedTree wt4 = wtree_crear(btree_conectar(4, NULL, NULL), 4);
    WeightedTree wt3 = wtree_crear(btree_conectar(3, NULL, NULL), 3);
    WeightedTree wt2 = wtree_crear(btree_conectar(2, NULL, NULL), 2);
    SList lista = NULL;
    lista = slist_agregar_inicio(lista, wt6);
    lista = slist_agregar_inicio(lista, wt4);
    lista = slist_agregar_inicio(lista, wt3);
    lista = slist_agregar_inicio(lista, wt2);
    BTree arbol = crear_arbol_final(lista);

    assert(arbol->data == -1);
    assert(arbol->left->data == -1);
    assert(arbol->right->data == 6);
    assert(arbol->left->left->data == -1);
    assert(arbol->left->left->right->data == 2);
    assert(arbol->left->left->left->data == 3);
    btree_destruir(arbol);   
}

void test_codigo_por_letra() {
    SList lista = NULL;
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(6, NULL, NULL), 6));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(4, NULL, NULL), 4));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(3, NULL, NULL), 3));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(2, NULL, NULL), 2));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(9, NULL, NULL), 1));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(10, NULL, NULL), 0));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(73, NULL, NULL), 0));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(65, NULL, NULL), 0));
    BTree arbol = crear_arbol_final(lista);

    char **codigos = malloc(sizeof(char*)*2);
    codigos[0] = malloc(sizeof(char)*15);
    codigos[1] = malloc(sizeof(char)*15);

    codigo_por_letra(arbol, 6, codigos[0] , 0);
    codigo_por_letra(arbol, 73, codigos[1] , 0);
    //arbol->right->data = 6
    //codigo_por_letra(arbol, 6, char* coordenada, 0) debería ser "1"
    assert(strcmp(codigos[0], "1") == 0);
    //arbol->left->left->left->right->right->left->left->data = 73
    //codigo_por_letra(arbol, 73, char* coordenada, 0) debería ser "0001100"
    assert(strcmp(codigos[1], "0001100") == 0);
    
    btree_destruir(arbol);
    free(codigos[0]);
    free(codigos[1]);
    free(codigos);
}

void test_traducir_a_codificado() {
    SList lista = NULL;
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(61, NULL, NULL), 6));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(71, NULL, NULL), 4));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(72, NULL, NULL), 3));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(125, NULL, NULL), 2));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(99, NULL, NULL), 1));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(107, NULL, NULL), 0));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(93, NULL, NULL), 0));
    lista = slist_agregar_inicio(lista, wtree_crear(btree_conectar(188, NULL, NULL), 0));
    BTree arbol = crear_arbol_final(lista);

    char **codigos = malloc(sizeof(char*)*256);
    codigos[61] = malloc(sizeof(char)*15);
    codigos[93] = malloc(sizeof(char)*15);
    codigos[71] = malloc(sizeof(char)*15);
    codigos[72] = malloc(sizeof(char)*15);
    codigo_por_letra(arbol, 61, codigos[61], 0);
    codigo_por_letra(arbol, 93, codigos[93], 0);
    codigo_por_letra(arbol, 71, codigos[71], 0);
    codigo_por_letra(arbol, 72, codigos[72], 0);
    char* asd = traducir_a_codificado("=]GH", 4, codigos);
    assert(strcmp(asd, "1000110001001") == 0);
    
    btree_destruir(arbol);
    free(codigos[61]);
    free(codigos[71]);
    free(codigos[72]);
    free(codigos[93]);
    free(codigos);
    free(asd);
}

/*   descompresor.c   */

void test_reconstruir_arbol_huff() {
    int pos = 0;
    char *arbolEnBinario = malloc(sizeof(char)*6);
    strcpy(arbolEnBinario, "01011");
    BTree arbolHuff = reconstruir_arbol_huff(arbolEnBinario, &pos);

    assert(arbolHuff->data == -1);
    assert(arbolHuff->left->data == 0);
    assert(arbolHuff->right->right->data == 0);
    btree_destruir(arbolHuff);
    free(arbolEnBinario);
}

void test_rellenar_arbol() {
    int pos = 0;
    char *arbolEnBinario = malloc(sizeof(char)*6);
    strcpy(arbolEnBinario, "01011");
    BTree arbolHuff = reconstruir_arbol_huff(arbolEnBinario, &pos);
    char *codigo1 = "0";
    rellenar_arbol(codigo1, 15, arbolHuff, 0);
    char *codigo2 = "10";
    rellenar_arbol(codigo2, 83, arbolHuff, 0);
    char *codigo3 = "11";
    rellenar_arbol(codigo3, 230, arbolHuff, 0);
    assert(arbolHuff->left->data == 15);
    assert(arbolHuff->right->left->data == 83);
    assert(arbolHuff->right->right->data == 230);
    assert(arbolHuff->right->data == -1);
    btree_destruir(arbolHuff);
    free(arbolEnBinario);
}

void test_decodificar_letra() {
    int pos = 0;
    char *arbolEnBinario = malloc(sizeof(char)*6);
    strcpy(arbolEnBinario, "01011");
    BTree arbolHuff = reconstruir_arbol_huff(arbolEnBinario, &pos);
    char *codigo1 = "0";
    rellenar_arbol(codigo1, 46, arbolHuff, 0);
    char *codigo2 = "10";
    rellenar_arbol(codigo2, 83, arbolHuff, 0);
    char *codigo3 = "11";
    rellenar_arbol(codigo3, 97, arbolHuff, 0);

    pos = 0;
    assert(decodificar_letra(arbolHuff, "0", &pos) == 46);
    pos = 0;
    assert(decodificar_letra(arbolHuff, "10", &pos) == 83);
    pos = 0;
    assert(decodificar_letra(arbolHuff, "11", &pos) == 97);
    
    btree_destruir(arbolHuff);
    free(arbolEnBinario);
}

void test_decodificar_string() {
    int pos = 0;
    char *arbolEnBinario = malloc(sizeof(char)*6);
    strcpy(arbolEnBinario, "01011");
    BTree arbolHuff = reconstruir_arbol_huff(arbolEnBinario, &pos);
    char codigo1[2] = "0";
    rellenar_arbol(codigo1, 46, arbolHuff, 0);
    char codigo2[3] = "10";
    rellenar_arbol(codigo2, 83, arbolHuff, 0);
    char codigo3[3] = "11";
    rellenar_arbol(codigo3, 97, arbolHuff, 0);

    int nuevaLen = 0;
    char *string1 = decodificar_string(arbolHuff, "01110", 5, &nuevaLen);
    assert(strcmp(string1, ".aS") == 0);
    nuevaLen = 0;
    char *string2 = decodificar_string(arbolHuff, "01110111111", 11, &nuevaLen);
    assert(strcmp(string2, ".aSaaa") == 0);
    
    btree_destruir(arbolHuff);
    free(arbolEnBinario);
    free(string1);
    free(string2);
}

int main() {
    test_slist_agregar_inicio();
    test_slist_agregar_final();
    test_slist_insertar_ordenado();
    test_btree_conectar();
    test_wtree_crear();
    test_crear_lista_warboles();
    test_crear_arbol_final();
    test_codigo_por_letra();
    test_traducir_a_codificado();
    test_reconstruir_arbol_huff();
    test_rellenar_arbol();
    test_decodificar_letra();
    test_decodificar_string();
    return 0;
}














