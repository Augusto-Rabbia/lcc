#include "syscall.h"
#include "lib.h"

unsigned strlen(const char *s) {
    unsigned i;
    for (i = 0; s[i] != '\0'; i++) {}
    return i;
}



void puts(const char* s) {
    unsigned len = strlen(s);
    Write(s, len, CONSOLE_OUTPUT);
    Write("\n", 1, CONSOLE_OUTPUT);
}


int power(int val, unsigned p) {
    if (p == 0) return 1;
    int res = val;
    for (int i = 1; i < p; ++i, res *= val){};
    return res;
}

void itoa(int value, char *sp) {
    unsigned digits = 0;
    short neg = 0;
    if (value < 0) {
        value = ~value+1;
        neg = 1;
    }

    int res = 0;
    do {
        res = value / power(10, ++digits);
    } while (res != 0);

    if (neg) {sp[0] = '-'; sp[digits+1] = '\0';}
    else {
        sp[digits] = '\0';
    }

    int i = digits-1;
    int j = 0;
    while (i >= 0) {
        int u = power(10, i);
        int n = (value / u);
        n -= j / u;
        j += n * u;

        if (neg) { sp[digits - i] = n + '0'; }
        else { sp[digits - i - 1] = n + '0'; }
        --i;
    }
}