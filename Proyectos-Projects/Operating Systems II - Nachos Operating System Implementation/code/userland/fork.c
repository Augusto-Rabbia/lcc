#include "syscall.h"


int
main(void)
{
    int i = 0;
    for (;i < 10000000; ++i){};
    // Not reached.
    return -1;
}
