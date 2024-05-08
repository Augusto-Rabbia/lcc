#include <stdio.h>
#include <stdlib.h>
#include <string.h> //para el 9

// ej7
void suma_dos(int* x, int* y, int z){
    *x = *x + 2;
    *y = *y + 5;
    z = z + 4;
}
// ej8
void ingreseCadena(char* c){
gets(c);
}
// ej 9
int *fun0(int x){
    return &x;
}


int main(){
    /* ej1 */
    int* punt;
    int x =7;
    int y = 5;
    punt =& x ;
    * punt = 4;
    printf (" %d %d", x , y );

    /* ej2
    int* punt;
    int x = 7;
    int y = 5;
    punt = &x;
    x = 4;
    punt = &y;
    printf (" %d %d", *punt, x); */

    /* ej3
    int* punt, i;
    int x[]={1, 2, 3, 4, 5};
    punt = x;
    *punt = 9;
    for(i = 0; i < 5; i ++){
        printf ("%d ", x[i]);
    } */

    /* ej4
    int* punt , i;
    int x[]={1 , 2 , 3 , 4 , 5};
    punt = x;
    *(punt + 2) = 9;
    *(x + 3) = 7;
    punt[1]=11;
    for(i = 0; i < 5; i ++){
        printf("%d ", *(punt + i));
    } */

    /* ej5 
    int* punt, i;
    int x[5] = {1 ,2 ,3 ,4 ,5};
    punt = &x[0]+3;
    *(punt - 2) =9;
    punt --;
    *(punt) = 7;
    punt[1] = 11;
    punt = x;
    for(i = 0; i < 5; i ++){
        printf("%d,", punt[i]);
    }*/

    /* ej6 
    int v[4] = {2, 4, 5, 7}, a , *p;
    p = v + 2;
    p --;
    a = *p + *(p + 1) + *(v + 1) + p[2];
    printf("%d ", a);*/
    
    /* ej7
    int x, y, z;
    x = 3;
    y = 10;
    z = 15;
    suma_dos (&x , &y, z) ;
    printf ("%d %d %d", x, y, z);*/

    /* ej8 
    char* cadena = malloc(sizeof(char)*10);
    gets(cadena);
    printf ("%s\n", cadena);
    ingreseCadena(cadena);
    printf ("%s", cadena);
    free(cadena);
    system("Pause");*/

    /* ej9

    int *ptr = NULL;
    ptr = fun0(2);
    printf("%d\n", *ptr);
    printf("%d\n", *ptr);*/
    return 0;
}


