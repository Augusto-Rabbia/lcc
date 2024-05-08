#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "funcs.h"

void test_generar_tablero_vacio() {
    char** test = generar_tablero_vacio(5);
    for(int i = 0; i < 5; i++ ) {
        assert(strcmp(test[i], "") == 0);
    }
}

//crear_tablero no puede ser testeado por contener un elemento rand()

int main(void) {
    test_generar_tablero_vacio();
    return 0;
}  