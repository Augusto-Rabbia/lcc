#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "huff_btree_slist.h"
#include "io.h"
#include "descompresor.h"

BTree reconstruir_arbol_huff(char* buffer, int *posicion) {
    if (buffer[*posicion] == '0') { //Reconstrucción de la forma de un arbol a
        (*posicion)++;              //partir de su representación Preorder en binario
        BTree izq = reconstruir_arbol_huff(buffer, posicion);
        BTree der = reconstruir_arbol_huff(buffer, posicion);
        return btree_conectar(-1, der, izq);
    }
    else { //Si encontramos un 1, colocamos una hoja.
        (*posicion)++;
        return  btree_conectar(0, NULL, NULL);
    }
}

void rellenar_arbol(char* buffer, int caracter, BTree arbolVacio, int orden) {
    if (buffer[orden] == '0') { //Recorremos el arbol en la dirección que indique el código asignado al caracter
        rellenar_arbol(buffer, caracter, arbolVacio->left, orden+1);
    }
    else {
        if (buffer[orden] == '1') {
        rellenar_arbol(buffer, caracter, arbolVacio->right, orden+1);
        }
        else { //Insertamos el caracter en la hoja en la que arribamos
            arbolVacio->data = caracter;
        }
    }
}

BTree recuperar_arbol_huff(char* treefilename) {
    FILE* f = fopen(treefilename, "r");
    int maxLineSize = 255, currentLength = 0;
    char* buffer = malloc(sizeof(char) * maxLineSize);
    char c;

    while ((c = getc(f)) != EOF && c != '\n') { // Obtención de la primera linea del archivo .tree
        if (currentLength == maxLineSize - 1) {
                maxLineSize *= 2;
                buffer = realloc(buffer, sizeof(char) * maxLineSize);
            }
            buffer[currentLength] = c;
            currentLength++;
    }

    int* posicion = malloc(sizeof(int));
    *posicion = 0;
    BTree huffTree = reconstruir_arbol_huff(buffer, posicion); // Arbol de Huffman vacío
    free(posicion);

    int letra = 0;
    while (fgets(buffer, 256, f) != NULL) { // Rellenado del Arbol de Huffman
        rellenar_arbol(buffer, letra, huffTree, 0);
        letra++;
    }

    free(buffer);
    fclose(f);
    return huffTree;
}

unsigned char decodificar_letra(BTree arbol, char* binario, int* aLeer) {
    if (arbol->data != -1) { // Si estamos en una hoja, devolvemos su valor
        return (unsigned char)arbol->data;
    }
    else if (binario[*aLeer] == '0') { // En caso contrario, recorremos el arbol en
            (*aLeer)++;                // la dirección que indique el string
            return decodificar_letra(arbol->left, binario, aLeer);
    }
    else {
        (*aLeer)++;
        return decodificar_letra(arbol->right, binario, aLeer);
    }
}

char* decodificar_string(BTree arbol, char* binario, int longitud, int* nuevaLongitud) {
    int size = 256;
    char *texto = malloc(sizeof(char)*size);
    int posicion = 0, index = 0;
    while(posicion < longitud) { //Iteración sobre el string binario
        if (index == size-2) {
            size *= 2;
            texto = realloc(texto, sizeof(char) * size);
        }
        texto[index] = decodificar_letra(arbol, binario, &(posicion));
        index++;
    }
    *nuevaLongitud = index;
    texto[index] = '\0';
    return texto;
}

void descomprimir(char* filename) {
    char treefilename[256]; // Obtención el nombre del archivo .tree
    strcpy(treefilename, filename);
    treefilename[strlen(filename)-2] = '\0';
    strcat(treefilename, "tree");

    BTree huffTree = recuperar_arbol_huff(treefilename); // Arbol de Huffman

    int lenComprimido;
    char* stringComprimido = readfile(filename, &(lenComprimido));

    int lenBinario;
    char* stringBinario = explode(stringComprimido, lenComprimido, &(lenBinario));

    int lenDescomprimido;
    char* descomprimido = decodificar_string(huffTree, stringBinario, lenBinario, &lenDescomprimido);

    char decFilename[256]; // Obtención del nombre del archivo .dec
    strcpy(decFilename, filename);
    decFilename[strlen(filename)-2] = '\0';
    strcat(decFilename, "dec");
    writefile(decFilename, descomprimido, lenDescomprimido);


    btree_destruir(huffTree);
    free(stringComprimido);
    free(stringBinario);
    free(descomprimido);
}
