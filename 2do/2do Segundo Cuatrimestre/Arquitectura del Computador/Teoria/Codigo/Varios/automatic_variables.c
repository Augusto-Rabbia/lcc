#include <stdio.h>

int a=5, b=10;

int main()
{
    int c=2, d=3;
    printf(" %d\n", a+b+c+d);
    printf(" Dirección de a: %p \n Dirección de c: %p \n Dirección de c: %p \n Dirección de d: %p\n",&a,&b,&c,&d);
    return 0;
}
