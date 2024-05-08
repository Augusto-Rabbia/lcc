#include <stdio.h>
#include <stdlib.h>
#include "Ej5funcs.h"

int main(void) {
    Matrix M1;
    int rowsM1 = 3, colsM1 = 5;
    M1 = matrix_create(rowsM1, colsM1);

    matrix_write(M1, 0, 0, 15);
    matrix_write(M1, 2, 4, 25);
    printf("%d, %d\n", matrix_read(M1, 0, 0), matrix_read(M1, 2, 4));

    return 0;
}