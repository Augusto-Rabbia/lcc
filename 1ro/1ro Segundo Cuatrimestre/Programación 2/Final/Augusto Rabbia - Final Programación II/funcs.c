#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "funcs.h"

void pedir_input(int* dimension) {
    printf("Ingrese INT: ");
    scanf("%d", dimension);
}

char** generar_tablero_vacio(int dim) {
    char** strarr = malloc(sizeof(char*) * dim);
    for (int i = 0; i < dim; i++) {
        strarr[i] = malloc(sizeof(char) * (dim - 1));
        strcpy(strarr[i], "");
    }
    return strarr;
}

void liberar_strarr(char** strarr, int dim) {
    for (int i = 0; i < dim; i++) {
        free(strarr[i]);
    }

    free(strarr);
}

char** crear_tablero(int dim) {
    char** tablero = generar_tablero_vacio(dim);
     
    for (int i = 0; i < dim * (dim - 1); i++) {
        int ficha = rand() % 3;
        if (ficha == 0) {
            strcat(tablero[i % dim], "0");
        }
        else if (ficha == 1) {
            strcat(tablero[i % dim], "1");
        }
        else {
            strcat(tablero[i % dim], "_");
        }
    }

    return tablero;
}

void imprimir_tablero(char** tablero, int dim) {
    FILE* archivoaEscribir = fopen("Cuatro_en_Linea.txt", "w");
    fprintf(archivoaEscribir, "DIMENSION\n%d\nTABLERO\n", dim);

    for (int i = 0; i < dim; i++) {
        fprintf(archivoaEscribir, "%s\n", tablero[i]);
    }
}
