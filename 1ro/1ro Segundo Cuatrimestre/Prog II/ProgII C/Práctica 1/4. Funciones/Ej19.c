#include <stdio.h>
#include <stdlib.h>

int fibonacci(int N){
    int order = 0;
    int a = 1;
    int b = 0;
    while(order < N){
        b += a;
        a = b-a;
        order ++;
        printf("%d ", a);
    }
    return a;
}

int main(){
    int Input;
    printf("Input an integer to find its value in the Fibonacci function: ");
    scanf("%d", &Input);
    printf("%d\n", fibonacci(Input));
    system("Pause");
    return 0;
}