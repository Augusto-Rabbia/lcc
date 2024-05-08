#include <stdio.h>

int main(){ 
	struct a {char a:1; char b:2; char c:1;};
	struct a valor;

	valor.a=0;
	valor.b=0;
	valor.c=1;
	//valor.d=2;

	printf("%ld\n", sizeof valor);
	printf("%d\n", valor.c);
	//printf("%d\n", valor.d);
	printf("%d\n", valor.a);
	printf("%hhx\n", *(char *) &valor);

	return 0;
} 

