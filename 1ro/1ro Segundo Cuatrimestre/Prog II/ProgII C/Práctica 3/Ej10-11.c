#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    /*char textoA [30] = "Agarrate Catalina";
    char textoB [30] = "El Cuarteto de Nos";
    char *pTextoA = textoA;
    char *pTextoB = textoB;
    printf ("textoA: %s\ntextoB: %s\n", textoA, textoB);
    while (*pTextoA ++ == *pTextoB ++);
    printf("while (*p++ = *q ++);\n");
    printf("textoA: %s\ntextoB: %s\n", textoA, textoB);
    Este código no tiene nada de puto sentido*/

   //Ej 11
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *pArray;
    pArray = array;
    printf ("array [0] > %d; *pArray: %d\n", array[0], *pArray);
    printf ("array [1] > %d; *(pArray +1): %d\n", array[1], *(pArray +1));
    pArray ++;
    printf ("pArray ++; *pArray: %d\n", *pArray);

    return 0;
}


