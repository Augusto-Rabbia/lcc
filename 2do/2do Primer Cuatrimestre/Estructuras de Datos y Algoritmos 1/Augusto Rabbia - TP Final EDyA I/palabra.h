#ifndef __PALABRA_H__
#define __PALABRA_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CORRECCIONES 3

#define CANT_MAX_SUGERENCIAS 5

struct _Palabra {
    char* str;
    int* lineas;
    int cantLineas;
    int lineasArrsize;
    char** sugerencias;
    int cantSugerencias;
};
typedef struct _Palabra *Palabra;

/**
 * Crea una palabra.
 */
Palabra palabra_crear(char* str, int linea);

/**
 * Destruye una Palabra.
 */
void palabra_destruir(Palabra palabra);

/**
 * Agrega una linea al array de lineas de la palabra.
 */
void palabra_agregar_linea(Palabra p, int linea);

/**
 * Añade el string al array de sugerencias de la Palabra.
 */
void palabra_agregar_sugerencia(Palabra palabra, char* sugerencia);

#endif