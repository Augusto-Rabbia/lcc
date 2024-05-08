#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

int A[N][N], B[N][N], C[N][N];

void mult(int A[N][N], int B[N][N], int C[N][N]) {
    int i, j, k;
    #pragma omp parallel for
    for (i = 0; i < N; i++)
        #pragma omp parallel for
        for (k = 0; k < N; k++)
            #pragma omp parallel for
            for (j = 0; j < N; j++)
                C[i][j] += A[i][k] * B[k][j]; //BT es [j][k]
}

int main() {

    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = random() % 1000;
            B[i][j] = random() % 1000;
        }
    }
    mult(A, B, C);
    return 0;
}