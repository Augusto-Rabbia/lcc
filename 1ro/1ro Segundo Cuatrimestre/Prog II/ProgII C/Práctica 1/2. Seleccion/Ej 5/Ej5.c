#include <stdio.h>
#include <stdlib.h>

int main(){
    int nota;
    printf("Qué nota has obtenido?: ");
    scanf("%d", &nota);
    if(nota < 6 && nota > 0) {printf("Insuficiente\n");}
    else switch(nota){
        case 6:
            printf("Aprobado\n");
            break;
        case 7:
            printf("Bueno\n");
            break;
        case 8:
            printf("Muy bueno\n");
            break;
        case 9:
            printf("Distinguido\n");
            break;
        case 10:
            printf("Sobresaliente\n");
            break;
        default:
            printf("Nota no válida\n");
        }
    system("Pause");
    return 0;
}
