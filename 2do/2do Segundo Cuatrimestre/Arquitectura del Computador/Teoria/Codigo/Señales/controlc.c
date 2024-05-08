#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void hand(int n)
{
    printf("En el handler\n");
    printf("Saliendo del handler\n");
}

int main(void)
{
    signal(SIGINT, hand);
    for (;;) {
        sleep(1);
        printf(".\n");
    }
    return 0;
}
