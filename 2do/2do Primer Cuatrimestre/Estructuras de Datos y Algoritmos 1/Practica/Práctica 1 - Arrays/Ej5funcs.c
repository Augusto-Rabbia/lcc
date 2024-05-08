#include <stdio.h>
#include <stdlib.h>
#include "Ej5funcs.h"

typedef int** Matrix;

Matrix matrix_create(int rows, int cols) {
    Matrix M;
    M = malloc(sizeof(int*)*rows);
    
    int i;
    for(i=0; i < rows; i++) {
        M[i] = malloc(sizeof(int)*cols);
    }
    return M;
}

void matrix_destroy(Matrix M, int rows) {
    int i;
    for(i=0; i < rows; i++) {
        free(M[i]);
    }
    free(M);
}

//e represents the element we want to insert into the matrix
void matrix_write(Matrix M, int row, int col, int e) {
    M[row][col] = e;
}

int matrix_read(Matrix M, int row, int col) {
    return M[row][col];
}