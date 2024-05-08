#include <stdio.h>

int main(int argc, char **argv, char **envp) {
	int i=0;

	printf("argc: %d, argv: %p, envp: %p\ndirs: %p %p %p, &i: %p\n", argc, argv, envp, &argc, &argv, &envp,&i);

	for(i=0;i<argc;i++) {
		printf("Arg: %d, dir: %p, apunta a: %p, valor: %s\n", i,&(argv[i]),argv[i],argv[i]);
	}

	for(;*envp;envp++) {
		printf("Envp: dir: %p, %s\n", envp, *envp);
	}
	
}
