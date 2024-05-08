#include <stdio.h>



int main(void) {
    int a = 1;
    a = a<<31;
    printf("%x", a);

    int b = 1;
    b = (b << 31) | (1 << 15);
    printf("\n%x", b);

    int c = -1;
    c = c & (-256);
    printf("\n%x", c);

    int d = 0xAA | (0xAA << 24);
    printf("\n%x", d);

    int e = 5 << 8;
    printf("\n%x", e);

    int f = -1 & (~(1 << 8));
    printf("\n%x", f);

    int g = 0 - 1;
    printf("\n%x", g);

    int h = 0x80000000 + 0x80000000;
    printf("\n%x", h);

    return 0;
}