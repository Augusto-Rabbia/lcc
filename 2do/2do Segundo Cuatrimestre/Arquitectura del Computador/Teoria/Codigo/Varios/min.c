#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

int min(int n, ...)
{
    va_list l;
    int min, next;

    va_start(l, n);
    if (n < 1) {
        va_end(l);
        return INT_MIN;
    };
    min = va_arg(l, int);
    for(unsigned i = 0; i < n - 1; i++) {
        next = va_arg(l, int);
        min = min < next ? min : next;
    }
    va_end(l);
    return min;
}

int main(void)
{
    printf("min(3, 2, 1, 3) -> %d\n", min(3, 2, 1, 3));
    printf("min(4, 3, 1, 2, 0) -> %d\n", min(4, 3, 2, 1, 0));
    return 0;
}
