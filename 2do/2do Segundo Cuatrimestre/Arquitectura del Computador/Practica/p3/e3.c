#include <stdio.h>

void sum(float *a, float *b, int len);

int main(){
    float a[3] = {1.0, 2.0, 3.0};
    float b[3] = {10.0, 20.0, 30.0};
    sum(a,b, 3);
    for(int i = 0; i < 3; i++) {
        printf("a[%d]=%f\nb[%d]=%f\n", i, a[i], i, b[i]);
    }
    return 0;
}
