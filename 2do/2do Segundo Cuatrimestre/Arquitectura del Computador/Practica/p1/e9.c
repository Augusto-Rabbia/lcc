#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int float_exp(float f) {
    return (* (int*) &f) >> 23 & 0x7f;
}

int float_frac(float f) {
    return (* (int*) &f) & 0x7fffff;
}

void printbin(int n) { 
    while(n != 0) {
        printf("%d", n & 1);
        n >>= 1;
    }
}

int main(void) {
    
    float f = 1.1;
    printf("%d, %d\n", float_exp(f), float_frac(f));

    printbin(* (int*) &f);
    puts("");
    printbin(float_frac(f));

    return 0;
}