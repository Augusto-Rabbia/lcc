#include <stdio.h>
#include <stdlib.h>


int main(){
    double result = 0;
    for(double i = 1; i <= 100; i++){
        result += 1 / i;
    }
    printf("%lf\n", result);
    system("Pause");
    return 0;
}