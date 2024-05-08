#include <stdio.h>
#include <stdlib.h>


int main(){
    int year;
    printf("Qué año es?: ");
    scanf("%d", &year);
    
    printf("%f\n", (year % 4));
    printf("%f\n", (year % 100));
    printf("%f\n", (year % 400));

    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {printf("Es un año bisiesto\n");}
        else {printf("No es un año bisiesto\n");};
    system("Pause");
    return 0;
    }