#include <stdio.h>
#include <stdlib.h>


int main(){
    int N1;
    int N2;
    printf("First number: ");
    scanf("%d", &N1);
    printf("Second number: ");
    scanf("%d", &N2);
    if(N1 < N2)
        {while(N1 <= N2) 
            {
            printf("%d\n", N1);
            N1 ++;
            }
        }
    else{printf("First number must be lower than the second\n");};
    system("Pause");
    return 0;
}