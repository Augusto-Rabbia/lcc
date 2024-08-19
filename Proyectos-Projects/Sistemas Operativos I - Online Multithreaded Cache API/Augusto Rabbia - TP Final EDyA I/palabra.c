#include "palabra.h"


Palabra palabra_crear(char* str, int linea) {
    Palabra p = malloc(sizeof(struct _Palabra));
    p->str = malloc(sizeof(char) * 256);
    strcpy (p->str, str);
    p->sugerencias = malloc(sizeof(char*) * CANT_MAX_SUGERENCIAS);
    p->lineas = malloc(sizeof(int) * 1);
    p->lineas[0] = linea;
    p->cantLineas = 1;
    p->lineasArrsize = 1;
    p->cantSugerencias = 0;
    return p;
}

void palabra_destruir(Palabra palabra) {
    for (int i = 0; i < palabra->cantSugerencias; i++) {
        free(palabra->sugerencias[i]);
    }
    free(palabra->sugerencias);
    free(palabra->str);
    free(palabra->lineas);
    free(palabra);
}

void palabra_agregar_linea(Palabra p, int linea) {
    if(p->cantLineas == p->lineasArrsize) {
        p->lineasArrsize *= 2;
        p->lineas = realloc(p->lineas, sizeof(int) * p->lineasArrsize);
    }
    p->lineas[p->cantLineas] = linea;
    p->cantLineas++;
}

void palabra_agregar_sugerencia(Palabra palabra, char* sugerencia) {
    palabra->sugerencias[palabra->cantSugerencias] = malloc(sizeof(char) * 256);
    strcpy(palabra->sugerencias[palabra->cantSugerencias], sugerencia);
    palabra->cantSugerencias++;
}
