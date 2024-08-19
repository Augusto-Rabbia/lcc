#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "huff_btree_slist.h"
#include "compresor.h"
#include "descompresor.h"

/**
 * Verifica que el input dado sea correcto.
 * El modo debe ser 'C' o 'D' y los únicos tipos de archivo
 * aceptado son .txt y .hf respectivamente para cada uno.
 */
void chequear_input(char* modo, char* filename) {
    if (strcmp(modo, "D") == 0) {
        char filename_type[256];
        int i = 0;
        while(filename[i] != '\0') {
            i++;
            if(filename[i] == '.') {
                strcpy(filename_type, &(filename[i]));
        }
    }
        if (strcmp(filename_type, ".hf") != 0) {
            printf("\nERROR. Debe ingresar un archivo .hf para descomprimir.");
            exit(1);
        }
    }
    else if (strcmp(modo, "C") != 0) {
        printf("\nERROR. Modo de ejecucion invalido.");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        chequear_input(argv[1], argv[2]);
        if (strcmp(argv[1], "C") == 0) {
            comprimir(argv[2]);
        }
        else if (strcmp(argv[1], "D") == 0) {
            descomprimir(argv[2]);
        }
    }
    else {
        printf("ERROR. Cantidad inválida de argumentos.");
        exit(1);
    }

    return 0;
}
