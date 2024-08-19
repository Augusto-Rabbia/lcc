#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "trie.h"
#include "tablaHash.h"
#include "palabra.h"

/**
 * Crea el trie con los las palabras en el archivo de entrada.
 */
Trie crear_trie_diccionario(char* dictFilename);

/**
 * Verifica si un string se encuentra en el Trie. Si no está,
 * lo añade al array de Palabras.
 * Observación: Solo lo busca un string si su largo es >= 2.
 * 
 * @param index Longitud del string.
 * @param cantIncorrectas Contador de palabras incorrectas.
 * @param linea Línea en la cual se encontró el string en el archivo de entrada.
 */
void verificar_incorrectas(Palabra** arrayPalabras, tablaHash incorrectasEncontradas, Trie trieDict, int index, int *arrSize, int* cantIncorrectas, char* str, int linea);

/**
 * Crea un array de estructura Palabra con las palabras del 
 * archivo que no se encuentren en el trie.
 */
Palabra* palabras_incorrectas(char* filename, int* cantIncorrectas, Trie trieDict);

/**
 * Busca todas las correcciones posibles a una palabra en pasosRestantes correcciones.
 * 
 * @param incorrecta La estructura de Palabra a modificar.
 * @param encontradas tablaHash que guarda todos los strings hallados durante la aplicación de
 * las funciones de corrección, además de la cantidad de pasos que le restaban aplicar.
 * De esta manera, si se vuelve a encontrar el mismo string siguiendo otra collección
 * de correcciones, se comparará a cuál se llegó en menos pasos.
 * Si se llegó a un mismo string en una cantidad mayor o igual de pasos, entonces se abortará
 * la recursión de buscar_correcciones en la función verificar_correccion_backtrack.
 * @param idxMaxModificacion Representa el máximo index hasta el que las modificaciones se
 * tienen que realizar en la próxima aplición de buscar_correcciones para que
 * exista una posibilidad de encontrar una nueva corrección.
 * @param pasosRestantes Representa la cantidad de correcciones que quedan por hacer.
 * Si es 0, se aborta la función y se detienen todos los for loops. 
 */
void buscar_correcciones(Trie trieDict, Palabra incorrecta, tablaHash *encontradas, int lenStr, int idxMinMod, int idxMaxMod, int pasosRestantes);

/**
 * Toma un string como entrada y devuelve el string con un espacio (" ")
 * en el index.
 */
char* separar_palabras(char* str, int indexSeparacion);

/**
 * Verifica si la corrección está o no en la tablaHash y si es un string válido
 * en el Trie y aplica la recursión de buscar_correcciones si es correcto.
 */
void verificar_correccion_recursion(Trie trieDict, Palabra incorrecta, tablaHash *encontradas, int lenStr, int idxMinMod, int pasosRestantes);

/**
 * Busca una palabra en el trie. 
 * Se utilizará en el programa únicamente para buscar una segunda palabra, 
 * llamada desde buscar_palabras_adyacentes.
 * Se diferencia de trie_buscar en que modifica el valor de idxMaxModAdy en
 * caso de encontrarse con que el trie termina.
 * 
 * @param idxMaxModAdy Representa el máximo index hasta el cual puede
 * modificarse la palabra buscada sin perderse posibles sugerencias, pues
 * luego de este index, el trie termina y no hay manera de formar una palabra
 * reconocida por el trie empezando desde ahí.
 */
bool buscar_palabra(Trie trieDict, unsigned char* str, int* idxMaxModAdy);

/**
 * Busca una o dos palabras adyacentes reconocidas por el trie. 
 * Luego las añade a la estructura Palabra si es correcto.
 */
void buscar_palabras_adyacentes(Trie trieDict, Palabra incorrecta, int* idxMaxMod, int pasosRestantes, int repetida);

/**
 * Intercambia la posición de la letras en index y index+1 en el string.
 */
void intercambiar_letras(char** str, int index);

/**
 * Cambia el caracter en el index del string por el caracter indicado.
 */
void modificar_letra(char** str, int index, char letra);

/**
 * Inserta el caracter indicado al string en el index. 
 */
void insertar_letra(char** str, int len, int index, char letra);

/**
 * Elimina el caracter del string en el index. 
 */
void eliminar_letra(char** str, int len, int index);

/**
 * Obtiene las correcciones de una palabra.
 */
void corregir_palabras(Trie trieDict, Palabra* palabrasIncorrectas, int cantIncorrectas);

/**
 * Escribe en el archivo indicado con la palabra y las correcciones de la misma.
 */
void escribir_archivo(Palabra* palabrasCorregidas, int cantIncorrectas, char* outputFileName);

/**
 * Convierte el string a uppercase.
 */
char* strupper(char** str);

#endif