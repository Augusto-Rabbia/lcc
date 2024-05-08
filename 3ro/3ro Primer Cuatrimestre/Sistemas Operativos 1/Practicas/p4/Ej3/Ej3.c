#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int primalidad(uint64_t num) {
    int retval = 1;
    uint64_t raiz = ceil(sqrt(num));
    #pragma omp parallel for
    for(uint64_t i = 2; i <= raiz; i++) {
        if ((num%i) == 0) {
            retval = 0; 
            #pragma omp flush(retval)
        }
        
        if (!retval) i = num;
    }
    return retval;
}

int main() {
    uint64_t num = 6894435075237291247;  
    printf("primo: %d\n", primalidad(num));
    return 0;
}
