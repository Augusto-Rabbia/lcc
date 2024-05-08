#include <stdio.h>

int solve(float a, float b, float c, float d, float e, float f, float* x, float* y);

int main(){
    float x, y;
    solve(1, 2, 5, 1, 3, 7, &x, &y);
    printf("x=%f, y=%f\n", x, y);
    return 0;
}
