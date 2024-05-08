#ifndef Ej5funcs_H
#define Ej5funcs_H

typedef int** Matrix;

Matrix matrix_create(int rows, int cols);

void matrix_destroy(Matrix M, int rows);


//e represents the element we want to insert into the matrix
void matrix_write(Matrix M, int row, int col, int e);

int matrix_read(Matrix M, int row, int col);

#endif
