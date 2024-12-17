#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "funcs.h"


int main(void) {
    srand(time(NULL));
    int dimension;

    pedir_input(&dimension);

    char** tablero = crear_tablero(dimension);

    imprimir_tablero(tablero, dimension);

    liberar_strarr(tablero, dimension);

    return 0;
}
