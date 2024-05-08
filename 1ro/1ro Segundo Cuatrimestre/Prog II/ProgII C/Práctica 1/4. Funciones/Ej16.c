#include <stdio.h>
#include <stdlib.h>

int max3(int N1, int N2, int N3){
    if(N1 > N2 && N1 > N3){return N1;}
    else if(N2 > N1 && N2 > N3){return N2;}
    else {return N3;}
}
int ladosTriangulo(int L1, int l2, int l3){
    if(L1 < l2 + l3){return 1;}
    else{return 0;}
}

int main(){
    int Num1, Num2, Num3;
    printf("Da tres enteros separados por una coma: ");
    scanf("%d,%d,%d", &Num1, &Num2, &Num3);
    int LadoMayor = max3(Num1, Num2, Num3);
    if(LadoMayor == Num1) {printf("%d\n",ladosTriangulo(Num1, Num2, Num3));}
    else if(LadoMayor == Num2) {printf("%d\n", ladosTriangulo(Num2, Num1, Num3));} 
    else {printf("%d\n", ladosTriangulo(Num1, Num2, Num3));}
    system("Pause");
    return 0;
}