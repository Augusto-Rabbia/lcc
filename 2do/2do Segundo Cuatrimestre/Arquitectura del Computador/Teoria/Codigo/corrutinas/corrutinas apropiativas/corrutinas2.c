#include "guindows.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static task t1, t2, tmain, tsched;
static int i;

#define ACTIVATE(d)  TRANSFER(tsched, d)
#define YIELD(o)  TRANSFER(o, tsched)



static void fsched(void)
{
    for (;;) {
        if (i++ % 2 != 0) {
            ACTIVATE(t1);
        } else {
            ACTIVATE(t2);
        }
    }
}

static void ft1(void)
{
    for (;;) {
        printf("t1\n");
	//YIELD(t1);
    }
}

static void ft2(void)
{
    for (;;) {
        printf("t2\n");
	//YIELD(t2);
    }
}

static void handler(int sig)
{
    alarm(1);
    if (i % 2 != 0) {
        YIELD(t2);
    } else {
        YIELD(t1);
    }
}

int main(void)
{
    stack(t1, ft1);
    stack(t2, ft2);
    stack(tsched, fsched);

    struct sigaction act;
    memset(&act, '\0', sizeof act);
    act.sa_handler = &handler;

    act.sa_flags = SA_NODEFER;
    if (sigaction(SIGALRM, &act, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

    alarm(1);
    TRANSFER(tmain, t1);
    return 0;
}
 
// MULTITAREA APROPIATIVA:
// Un sistema operativo moderno suele usar un mecanismo similar (basado en
// temporizadores e interrupciones) para proveer multitarea. A esto se le
// llama multitarea apropiativa. Los procesos no se enteran ni tienen que
// hacer nada para convivir con otros procesos). El SO cambia de tarea de
// prepo (“preempt”) y de forma transparente.
