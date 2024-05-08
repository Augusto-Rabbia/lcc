#include <stdio.h>
#include <stdlib.h>

#define N 1000

int main(){
    int i, j, a[N+1]; //Declaracion enteros y arreglo de enteros. 
    for (a[1] = 0 , i = 2; i <= N; i ++) {
        a[i] = 1;
    }
    for (i = 2; i <= N/ 2; i ++) {
        for (j = 2; j*i <= N; j ++) {
            a[i*j] = 0;
        }
    }
    for (i = 1; i <= N; i ++){
        if (a[i]){ //Pasamos sólo los números que no sean múltiplos de otros 2 (hay valores de 1 en donde no haya 2 números que multiplicados den este) 
        printf("%d ", i); //imprime el valor que corresponde al 1.
        }
    }
    printf("\n");
    return 0;
}