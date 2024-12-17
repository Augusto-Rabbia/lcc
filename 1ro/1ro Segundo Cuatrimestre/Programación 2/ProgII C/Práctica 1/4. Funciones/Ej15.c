#include <stdio.h>
#include <stdlib.h>

int max(int N1, int N2){
    if(N1 > N2){return N1;}
    else{return N2;}
}

int main(){
    int Num1, Num2;
    printf("Da dos enteros separados por una coma: ");
    scanf("%d,%d", &Num1, &Num2);
    printf("El entero más alto entre esos dos es %d\n", max(Num1, Num2));
    system("Pause");
    return 0;
}