#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b){
    int c = *a;
    *a = *b;
    *b = c;
}

int main(){
    int a = 0, b = 1;
    int *pA = &a;
    int *pB = &b;
    swap(pA, pB);
    printf("a vale %d y b vale %d", a, b);
    return 0;
}