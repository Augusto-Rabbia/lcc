#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void hand(int n)
{
    printf("En el handler\n");
    alarm(2);
    sleep(3);
    printf("Saliendo del handler\n");
}

int main(void)
{
    signal(SIGALRM, hand);
    alarm(5);
    for (;;) {
        sleep(1);
        printf(".\n");
    }
    return 0;
}
