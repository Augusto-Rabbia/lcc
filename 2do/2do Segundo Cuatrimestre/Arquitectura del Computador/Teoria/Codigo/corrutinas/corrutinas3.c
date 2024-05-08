#include <stdio.h>
#include <signal.h>
#include "guindows2.h"
#include <string.h>
#include <unistd.h>

task t1, t2, t3, tmain, tsched;
void stack(task, void (*)());

static int i;

void fsched(){
	printf("Planificador, i: %d\n",i);
	for(;;){
		switch(i%3) {
			case 0: i++; ACTIVATE(t2); break;
			case 1: i++; ACTIVATE(t3); break;
			case 2: i++; ACTIVATE(t1); break;
		}
	}
}

void ft1(){
	for(;;)
		printf("t1\n");
}

void ft2(){
	for(;;)
		printf("t2\n");
}

void ft3(){
	for(;;)
		printf("t3\n");
}

void handler(int sig){
	alarm(1);
	switch(i%3) { 
		case 0: YIELD(t1); break;
		case 1: YIELD(t2); break;
		case 2: YIELD(t3); break;
	}
	
}

int main(void) {
	stack(t1, ft1);
	stack(t2, ft2);
	stack(t3, ft3);
	stack(tsched, fsched);

	struct sigaction act;
	memset(&act,'\0', sizeof(act));
        act.sa_handler = &handler;

        act.sa_flags = SA_NODEFER;
        if (sigaction(SIGALRM, &act, NULL) < 0) {
                perror ("sigaction");
                return 1;
        }

	alarm(1);
	TRANSFER(tmain, t1);
	return 0;
}
 
//MULTITAREA APROPIATIVA
//un sistema op moderno suele usar un mecanismo similar (basado en temporizadores e interrupciones) para proveer multitarea. A esto se le llama multitarea apropiativa
//(Los procesos no se enteran ni tienen que hacer nada para convivir con otros procesos).
//El SO cambia de tarea de prepo (preempt) y de forma transparente.