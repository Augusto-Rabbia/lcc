#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


int denom = 0;
void handler(int s) { printf("ouch!\n"); denom = 1; }
int main() {
    int r;
    signal(SIGFPE, handler);
    r = 1 / denom;
    printf("r = %d\n", r);
    return 0;
}

/*
 * Al correr el programa, el programa en un loop imprimiendo "ouch!" 
 * La razon por la que ocurre esto es la manera en que funcionan los handlers.
 * Al terminar las instrucciones del handler, se vuelve no a la siguiente linea de codigo,
 * sino que a la siguiente instruccion. Dependiendo de que nivel de optimizacion -O utilicemos,
 * esta instruccion sera la division entre dos registros, uno con el valor 1 y otro con el valor 0.
 * A pesar de que el dato guardado en la direccion de denom no es 0, el valor del registro que
 * el compilador esta utilizando para realizar la division no habra cambiado.
*/