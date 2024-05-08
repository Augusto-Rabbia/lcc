#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int float_exp(float n) {
    return (int) (char) (*((int *)&n) >> 23) -127;
}

int myisnan(float n) {
    float ftemp = NAN;
    
    return *((int *)&ftemp) == *((int *)&n);

    int nantemp = 0x7f800001;
    int ftoint = *((unsigned int *)&n);
    return nantemp <= ftoint;
}

int main(void) {
    float f = 1.0;
    printf("%d", float_exp(f));

    f = NAN;
    printf("\n%d", myisnan(f));
    return 0;
}