#include <stdio.h>
#include <stdlib.h>


int main(){
    int a = 20, b = 30;
    for(int i = 1; i <= a; i ++){
        for(int j = 1; j <= b; j ++){
            printf("%d + %d = %d\n", i*i, j*j, i*i+j*j);
        }
    }
    system("Pause");
    return 0;
}