#include <stdio.h>
#include <stdlib.h>


int main(){
    printf("Hello World!\n");
/*Ejercicio 1*/
    int a, b, c, d=6, e;
    a = b = 3;
    c = a*b+d;
    e = (c + 5) / 4-3;
    e+=5;
    printf("los resultados son %d y %d\n", c, e);
    system("Pause");
    return 0;
}