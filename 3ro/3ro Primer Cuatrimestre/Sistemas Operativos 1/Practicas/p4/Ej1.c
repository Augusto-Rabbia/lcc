#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double suma = 0;
double arreglo[500000000];


int main() {
    //omp_set_num_threads(3);
    #pragma omp parallel for
    for(int i = 0; i < 500000000; i++) {
        arreglo[i] = (double) (1);
    }

    #pragma omp parallel for reduction(+:suma)
    for(int i = 0; i < 500000000; i++) {
        suma += arreglo[i];
    }



    printf("suma:%lf\n", suma);    
    return 0;
}
