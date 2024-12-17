#include <stdio.h>
#include <stdlib.h>


int main (){
    float x, y;
    printf ("Introduzca 2 números:\n");
    scanf ("%f %f", &x, &y);
    printf ("La suma de %f y %f vale %f\n", x, y, x+y);
    printf ("La suma de %4f y %4.2f vale %10.3f\n", x, y, x+y);
    printf ("La suma de %e y %e vale %e\n", x, y, x+y);
    system("Pause");
    return 0;
}