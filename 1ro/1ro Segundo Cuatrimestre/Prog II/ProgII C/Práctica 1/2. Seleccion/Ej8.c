#include <stdio.h>
#include <stdlib.h>


int main(){
    int Edad;
    printf("Qué edad tienes?: ");
    scanf("%d", &Edad);
    if (Edad > 64) {printf ("Seguridad Social\n");}
        else if (Edad < 18) {printf("Exento\n");}
            else {printf("Imposible\n");}

    system("Pause");
    return 0;
}