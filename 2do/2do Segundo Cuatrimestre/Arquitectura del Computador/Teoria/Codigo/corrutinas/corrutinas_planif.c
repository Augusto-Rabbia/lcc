#include <stdio.h>
#include "guindows.h"

task t1, t2, taskmain, tsched;
int i=1;

void fsched(){
	for(;;) {
		if (i%2)  TRANSFER(tsched,t1)
		else      TRANSFER(tsched,t2);
		i++;
	}
}

void ft1(){
	double d;
	for(d=-1;;d+=0.001) {
		printf("d=%f\n", d);
		TRANSFER(t1,tsched);
	}
}

void ft2(){
	int i;
	for(i=0;i<10000;i++) {
		printf("i=%d\n", i);
		TRANSFER(t2,tsched);
	}
	TRANSFER(t2, taskmain);
}

int main(){
	stack(t1,ft1);
	stack(t2,ft2);
	stack(tsched,fsched);
	TRANSFER(taskmain,tsched);
	return 0;
}