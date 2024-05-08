#include <stdio.h>
#include "guindows.h"

#define ACTIVATE(t) TRANSFER(tsched,t)
#define YIELD(t)    TRANSFER(t,tsched)

task t1, t2, taskmain, tsched;
int i=0;

void fsched(){
	for(;;) {
		if (i%2)  ACTIVATE(t1)
		else      ACTIVATE(t2)
		i++;
	}
}

void ft1(){
	double d;
	for(d=-1;;d+=0.001) {
		printf("d=%f\n", d);
		YIELD(t1);
	}
}

void ft2(){
	int i;
	for(i=0;i<10000;i++) {
		printf("i=%d\n", i);
		YIELD(t2);
	}
	TRANSFER(t2, taskmain);
}

int main(){
	stack(t1,ft1);
	stack(t2,ft2);
	stack(tsched,fsched);
	TRANSFER(taskmain,t1);
	return 0;
}