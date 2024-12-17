#include <stdio.h>
#include <stdlib.h>


int main(){
    int Input;
    printf("Introduzca un número de un dado: ");
    scanf("%d", &Input);
    switch(Input){
    case 1: 
    printf("Hay un seis del lado opuesto\n");
    break;
    case 2: 
    printf("Hay un cinco del lado opuesto\n");
    break;
    case 3: 
    printf("Hay un cuatro del lado opuesto\n");
    break;
    case 4: 
    printf("Hay un tres del lado opuesto\n");
    break;
    case 5: 
    printf("Hay un dos del lado opuesto\n");
    break;
    case 6: 
    printf("Hay un 1 del lado opuesto\n");
    break;
    default: printf("Número inválido.\n");
    }
    system("Pause");
    return 0;
}