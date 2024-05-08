#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "huff_btree_slist.h"
#include "io.h"
#include "compresor.h"

WeightedTree wtree_crear(BTree arbolAInsertar, int pesoAInsertar) {
    WeightedTree new_wtree = malloc(sizeof(WTnode));
    new_wtree->arbol = arbolAInsertar;
    new_wtree->peso = pesoAInsertar;
    return new_wtree;
}

int* contar_letras(char* texto, int len) {
    int* listChars = calloc(sizeof(int), 256);

    for (int i = 0; i < len; i++) { // Sumamos las apariciones de cada caracter
        listChars[(unsigned char) texto[i]]++;
    }
    return listChars;
}

SList crear_lista_warboles(int* arr) {
    SList listTrees = slist_crear();
    for (int i = 0; i < 256; i++) { // Por cada caracter creamos un nodo con un WeightedTree.
        WeightedTree wtree = wtree_crear(btree_conectar(i, NULL, NULL), arr[i]);
        listTrees = slist_insertar_ordenado(listTrees, wtree);
    }
    return listTrees;
}

BTree crear_arbol_final(SList list) {
    SList temp = list;
    while (temp->sig != NULL) {    // Iteramos hasta que solo quede un nodo en la lista.
        WeightedTree listTree = list->wtree;      // Tomamos los dos primeros elementos de la lista
        WeightedTree nextTree = list->sig->wtree;
        temp = list->sig->sig; // Guardamos el tercer elemento de la lista.
        WeightedTree nuevoWTree = wtree_crear(btree_conectar(-1, listTree->arbol, nextTree->arbol),
            (listTree->peso + nextTree->peso)); //Unimos los dos primeros WTrees, sumando su peso
        temp = slist_insertar_ordenado(temp, nuevoWTree);
                            //Insertamos el nuevo WTree en la lista en orden según su peso
        free(listTree);
        free(nextTree);
        free(list->sig);
        free(list);
        list = temp;
    }
    BTree finalTree = list->wtree->arbol;
    free(list->wtree);
    free(list);
    return finalTree;
}

int codigo_por_letra(BTree arbol, int character, char* code, int posicion) {
    if (arbol != NULL) {
        if (codigo_por_letra(arbol->left, character, code, posicion+1)) {
            code[posicion] = '0'; // Si se encontró la letra en el subarbol
            return 1;             // izquierdo, agregamos "0" a su código
        }
        else {
            if (codigo_por_letra(arbol->right, character, code, posicion+1)) {
                code[posicion] = '1'; // Si se encontró la letra en el subarbol
                return 1;             // derecho, agregamos "1" a su código
            }
        }
        if (arbol->data == character) {
            code[posicion] = '\0';
            return 1; // Si encontramos el caracter, devolvemos True   
        }
    }
    return 0;
}

void liberar_str_arr(char** strarr, int len) {
    for (int i = 0; i < len; i++) {
        free(strarr[i]);
    }
    free(strarr);
}

char* traducir_a_codificado(char* text, int len, char** codesArray) {
    int size = len;
    char* strBinary = malloc(sizeof(char) * size);
    strBinary[0] = '\0';
    int j = 0;
    int k = 0;
    for(int i = 0; i < len; i++) { // Iteramos sobre el texto original
        j += strlen(codesArray[(unsigned char) text[i]]); // j contará el largo del string binario actual
        if (j >= size - 1) {
            size = 2 * j;
            strBinary = realloc(strBinary, sizeof(char) * size);
        }
        strcat(strBinary+k, codesArray[(unsigned char) text[i]]);
        // Concatenamos el código binario asociado a la letra por la que se está iterando.
        k = j;
    }
    //strBinary[k] = '\0';
    return strBinary;
}

void escribir_arbol_en_archivo(BTree arbol, FILE* f) {
    if (arbol != NULL) {
        if (arbol->data == -1)
        fputc('0', f);
    else
        fputc('1', f);

    escribir_arbol_en_archivo(arbol->left, f);
    escribir_arbol_en_archivo(arbol->right, f);
    }
}

void escribir_codigos_en_archivo_tree(char** codesArray, FILE* f) {
    for(int i = 0; i < 256; i++) {
        fprintf(f, "\n%s", codesArray[i]);
    }
}

void comprimir(char* filename) {
    int len;
    char* fileString = readfile(filename, &len);

    int* charFrequencyArr = contar_letras(fileString, len); // Array que relaciona las frecuencias con cada letra

    SList charTreeList = crear_lista_warboles(charFrequencyArr); // Lista simplemente enlazada con árboles que contiene cada caracter y su peso

    BTree huffTree = crear_arbol_final(charTreeList); // Arbol de Huffman

    char** charCodesList = malloc(sizeof(char*) * 256);   // Array que establece la relación entre cada caracter y su código
    for (int i = 0; i < 256; i++) {     // Creación de charCodesList
        charCodesList[i] = malloc(sizeof(char) * 256);
        codigo_por_letra(huffTree, i, charCodesList[i], 0);
    }
    char* binaryString = traducir_a_codificado(fileString, len, charCodesList);    // String binario a comprimir

    int lenBinaryString = strlen(binaryString);
    int lenCompressed;
    char* compressed = implode(binaryString, lenBinaryString, &lenCompressed);  // String comprimido

    free(charFrequencyArr);
    free(fileString);
    free(binaryString);
    /* Creación del archivo .hf */

    char hfFilename[256];
    strcpy(hfFilename, filename);
    strcat(hfFilename, ".hf");
    writefile(hfFilename, compressed, lenCompressed);

    free(compressed);
    /* Guardando del arbol binario */

    char treeFilename[256];
    strcpy(treeFilename, filename);
    strcat(treeFilename, ".tree");
    FILE* archivo_arbol = fopen(treeFilename, "wb");

    escribir_arbol_en_archivo(huffTree, archivo_arbol);

    escribir_codigos_en_archivo_tree(charCodesList, archivo_arbol);

    btree_destruir(huffTree);
    liberar_str_arr(charCodesList, 256);
    fclose(archivo_arbol);
}
