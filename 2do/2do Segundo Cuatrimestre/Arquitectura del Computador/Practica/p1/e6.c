#include <stdio.h>
#include <stdlib.h>

unsigned mult(unsigned a, unsigned b) {
    if (b == 0)
        return 0;
    if (b == 1) 
        return a;
    if ((b & 2) >> 1)
        return 2 * mult(a, b >> 1);
    return (2 * mult(a, b >> 1)) + a;
}

int main(void) {
    //printf("%d", 5 & 1);
    printf("%u", mult(5,10));
    return 0;
}