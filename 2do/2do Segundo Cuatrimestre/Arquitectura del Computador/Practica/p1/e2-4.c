#include <stdio.h>

int is_one(long n, int idx) {
    return (n << (sizeof(long)*8 - idx)) < 0;
}

void printbin(int n) { 
    while(n != 0) {
        printf("%d", n & 1);
        n >>= 1;
    }
}

void interchange(int* a, int* b, int* c) {
    *a = *a ^ *b ^ *c; 
    *b = *a ^ *c ^ *b; // b = (a ^ b ^ c) ^ c ^ b = a
    *c = *b ^ *a ^ *c; // c = a ^ (a ^ b ^ c) ^ c = b
    *a = *c ^ *b ^ *a; // a = c ^ b ^ (a ^ b ^ c) = c

    printf("%d, %d, %d", *a, *b, *c);
}

int main(void) {
    /*printf("%I64u", sizeof(long));
    printf("\n%d", is_one(8, 4));
    puts("");
    printbin(0xfaf);*/

    int a = 4, b = 8, c = 16;
    interchange(&a, &b, &c);
    return 0;
}