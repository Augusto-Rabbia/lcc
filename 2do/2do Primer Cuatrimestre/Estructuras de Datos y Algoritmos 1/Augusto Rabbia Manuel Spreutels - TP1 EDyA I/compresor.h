#ifndef __COMPRESOR_H__
#define __COMPRESOR_H__

#include "huff_btree_slist.h"
/**
 * Crea un WeightedTree con el arbol y peso deseado.
 */
WeightedTree wtree_crear(BTree arbolAInsertar, int pesoAInsertar);

/**
 * Recibe un string y devuelve un array de ints con la frecuencia de
 * cada letra en la posición que le corresponde según su código ASCII
 */
int* contar_letras(char* str, int len);

/**
 * Crea una lista simplemente enlazada de WeightedTrees, ordenados
 * del menor a mayor peso, cada uno con la letra que representan
 * como dato e hijos vacíos.
 */
SList crear_lista_warboles(int* arr);

/**
 * Crea el arbol de Huffman y libera la memoria dedicada a los
 * WeightedTrees y la lista que los contenía.
 */
BTree crear_arbol_final(SList lista);

/**
 * Devuelve la codificación de la letra insertada como argumento
 * recorriendo el arbol de Huffman.
 * Devuelve el código invertido, se le deberá aplicar string_reverse
 * posicion: La funcion deberá ser llamada siempre con posicion = 0.
 * posicion cuenta los caracteres que se están insertando.
 */
int codigo_por_letra(BTree arbol, int letra, char* coordenada, int posicion);

/**
 * Libera la memoria dedicada a un array de strings.
 */
void liberar_str_arr(char** strarr, int len);

/**
 * Recibe un string y devuelve el mismo codificado con la codificación
 * que reciba. A este se le deberá aplicar la función "explode" antes
 * de escribir en un archivo.
 */
char* traducir_a_codificado(char* string, int len, char** codificacion);

/**
 * Escribe la serialización del arbol de Huffman en el archivo de
 * destino.
 */
void escribir_arbol_en_archivo(BTree arbol, FILE* f);

/**
 * Escribe la codificación de Huffman de cada letra en orden en
 * el archivo de destino.
 */
void escribir_codigos_en_archivo_tree(char** codesArray, FILE* f);

/**
 * Coordina todas las funciones utilizadas para comprimir los datos
 * de un archivo y escribirlos en un archivo .hf, guardando la
 * información de la codificación utilizada en un archivo .tree.
 */
void comprimir(char* filename);

#endif
