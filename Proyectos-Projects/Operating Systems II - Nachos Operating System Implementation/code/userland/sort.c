/// Test program to sort a large number of integers.
///
/// Intention is to stress virtual memory system.
///
/// Ideally, we could read the unsorted array off of the file system,
/// and store the result back to the file system!
#include "syscall.h"

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


#define DIM 1024

/// Size of physical memory; with code, we will run out of space!
static int A[DIM];

int
main(void)
{
    int i, j, tmp;
    char buf[4];
    // First initialize the array, in reverse sorted order.
    for (i = 0; i < DIM; i++) {
        A[i] = DIM - i;
    }

    // Then sort!
    for (i = 0; i < DIM - 1; i++) {
        for (j = 0; j < DIM - 1 - i; j++) {
            if (A[j] > A[j + 1]) {  // Out of order -> need to swap!
                tmp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = tmp;
            }
        }
        
    }

    
    itoa(A[0], buf);
    puts(buf);
    itoa(A[1], buf);
    puts(buf);
    // And then we're done -- should be 0 (1)! (estaba mal y lo tuvimos que arreglarXD)
    return A[0];
}
