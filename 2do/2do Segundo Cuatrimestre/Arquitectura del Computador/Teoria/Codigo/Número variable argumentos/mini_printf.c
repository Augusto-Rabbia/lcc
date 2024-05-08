#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// `printf` simplificada. `fmt` puede tener:
//   i=>integer, c=>char, s=>string
void mini_printf(char *fmt, ...)
{
    va_list l;
    int i;
    char c, *s;

    va_start(l, fmt);
    for (char *p = fmt; *p; p++) {
        switch (*p) {
        case 'i':
            i = va_arg(l, int);
            printf("%d ", i);
            break;
        case 's':
            s = va_arg(l, char *);
            printf("%s ", s);
            break;
        case 'c':
            c = va_arg(l, int);  // `char` se promueve a `int`.
            printf("%c ", c);
            break;
        default:
            abort();
        }
    }
    printf("\n");
    va_end(l);
}

int main(void)
{
    mini_printf("iscc", 10, "hola", 'A', 'z');
    mini_printf("sic", "uno", 9, 'c');
}
