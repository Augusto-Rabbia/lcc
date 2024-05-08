#include <stdio.h>
#include <stdlib.h>


int main(){
    int Num;
    int Comparing = 2;
    int Prime = 0;
    printf("Input a number to know if it is prime: ");
    scanf("%d", &Num);
    while(Comparing <= Num / 2)
        {
        if(Num % Comparing == 0) 
            {
            printf("The number is not a prime number\n");
            Prime = 1;
            break;
            }
        Comparing ++;
        }
    if(Prime == 0) {printf("The number is a prime number\n");}
    system("Pause");
    return 0;
}