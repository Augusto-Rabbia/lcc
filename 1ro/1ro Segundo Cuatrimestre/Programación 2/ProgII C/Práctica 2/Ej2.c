#include <stdio.h>
#include <stdlib.h>


int main(){
    printf("Seleccione una habitación por su número: \nHabitación   Camas   Planta\n1.Azul       2       Primera\n2.Roja       1       Primera\n3.Verde      3       Segunda\n4.Rosa       2       Segunda\n5.Gris       1       Tercera\n: ");
    int Seleccion;
    scanf("%d", &Seleccion);
    switch(Seleccion){
        case 1:
        printf("Primera planta, 2 camas\n");
        break;
        case 2:
        printf("Primera planta, 1 cama\n");
        break;
        case 3:
        printf("Segunda planta, 3 camas\n");
        break;
        case 4:
        printf("Segunda planta, 2 camas\n");
        break;
        case 5:
        printf("Tercera planta, 1 cama\n");
        break;
        default: printf("Número invalido\n");
    }
    system("Pause");
    return 0;
}