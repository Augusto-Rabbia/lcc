#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printbin(unsigned long long n) {
    int i = 0;
    while(i < 32) {
        printf("%I64u", n & 1);
        n >>= 1;
        i++;
    }
}

int main(void) {
    double f = 6.02252 * pow(10, 23);
    printf("%f\n", f);
    printbin(*(unsigned long long*) &f);


    
    return 0;
}