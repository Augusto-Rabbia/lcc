#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int max3(int N1, int N2, int N3){
    if(N1 > N2 && N1 > N3){return N1;}
    else if(N2 > N1 && N2 > N3){return N2;}
    else {return N3;}
}
int esRectangulo(int L1, int L2, int L3){
    if(L1 == max3(L1, L2, L3)){if(L1 == sqrt(L2*L2+L3*L3)){return 1;}
                                      else{return 0;}}
    else if(L2 == max3(L1, L2, L3)){if(L2 == sqrt(L1*L1+L3*L3)){return 1;}
                                      else{return 0;}}
    else {if(L3 == sqrt(L1*L1+L2*L2)){return 1;}
                                      else{return 0 ;}}
}


int main(){
    int Num1, Num2, Num3;
    printf("Da tres enteros separados por una coma: ");
    scanf("%d,%d,%d", &Num1, &Num2, &Num3);
    printf("%d\n", esRectangulo(Num1, Num2, Num3));
    system("Pause");
    return 0;
}