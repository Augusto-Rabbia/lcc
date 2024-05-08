#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SQUARE(X)   X

int main(void)
{
    int a = SQUARE('y');
    printf("%d", a);
    return 0;
}