#include "guindows.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



static task t1, t2,t3, tmain, tsched;
static int i;

#define ACTIVATE(d)  TRANSFER(tsched, d)
#define YIELD(o)  TRANSFER(o, tsched)

static void fsched(void)
{

    for (;;) {
        i++;
	switch (i%3){ 
		case 0: ACTIVATE(t1);break;
            	case 1: ACTIVATE(t2);break;
            	case 2: ACTIVATE(t3);break;
        }
    }
}

static void ft1(void)
{
    for (;;) {
        printf("t1\n");
	YIELD(t1);
    }
}

static void ft3(void)
{
    for (;;) {
        printf("t3\n");
	YIELD(t3);
    }
}

static void ft2(void)
{
    for (;;) {
        printf("t2\n");
	YIELD(t2);
    }
}


int main(void)
{
    stack(t1, ft1);
    stack(t2, ft2);
    stack(t3, ft3);
    stack(tsched, fsched);

    TRANSFER(tmain, t1);
    return 0;
}
 
