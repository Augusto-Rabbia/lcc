#ifndef __DESCOMPRESOR_H__
#define __DESCOMPRESOR_H__

#include "huff_btree_slist.h"

/**
 * Recibe un string binario representante del recorrido preorden de un
 * arbol binario completo y lo reconstruye, sin escribir
 * datos en sus hojas.
 * posicion: pointer a int que siempre deberá ser 0 a la hora de llamar
 * a la función. 
 */
BTree reconstruir_arbol_huff(char* buffer, int *posicion);

/**
 * Recibe la codificación de una letra y recorre el arbol para
 * encontrar su hoja y escribe en esta la letra representada por el
 * código
 * orden: posición del string binario que estamos leyendo.
 */
void rellenar_arbol(char* buffer, int caracter, BTree arbolVacio, int orden);

/**
 * Lee un archivo .tree y devuelve el arbol de Huffman que tenga
 * escrito.
 */
BTree recuperar_arbol_huff(char* treefilename);

/**
 * Recibe un string binario y recorre el arbol de Huffman siguiendo el
 * recorrido indicado por el string, luego, devuelve el caracter ubicado
 * en la hoja a la que llegue.
 */
unsigned char decodificar_letra(BTree arbol, char* binario, int* aLeer);
/**
 * Recibe un string binario y lo recorre, decodificándolo de letra por
 * letra llamando a la función decodificar letra.
 */
char* decodificar_string(BTree arbol, char* binario, int longitud, int* nuevaLongitud);

/** xddd
 * Coordina todas las funciones utilizadas para descomprimir los datos
 * de un archivo .hf y escribirlos en un archivo .txt.
 */
void descomprimir(char* filename);

#endif
