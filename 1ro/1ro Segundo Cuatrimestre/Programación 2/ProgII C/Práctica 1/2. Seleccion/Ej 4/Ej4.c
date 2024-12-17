#include <stdio.h>
#include <stdlib.h>


int main(){
    float t;
    printf("A qué temperatura está el agua?: ");
    scanf("%f", &t);
    if (t <= 0) {printf("El agua está en estado sólido.");}
    else if (t < 100) {printf("El agua está en estado líquido.");}
    else {printf("El agua está en estado gaseoso.");}
    system("Pause");
    return 0;
}