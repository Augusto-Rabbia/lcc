#include <stdio.h>
#include <stdlib.h>


int main(){
    int Pacientes;
    printf("Cuántos pacientes hay?: ");
    scanf("%d", &Pacientes);

    int PacAn = 0;
    int PacIng = 0;
    int PacOp = 0;

    int SumEdadAnalizados = 0;
    int SumEdadIngresados = 0;
    int SumEdadOperados = 0;
    int Edad;
    float Indice;

    while(Pacientes != 0){
        printf("Edad del Paciente %d: ", Pacientes);
        scanf("%d", &Edad);
        printf("Índice del Paciente %d: ", Pacientes);
        scanf("%f", &Indice);

        if(Indice < 0.6) {
            SumEdadAnalizados = SumEdadAnalizados + Edad;
            PacAn ++;
            }
        else if(Indice < 0.9) {
            SumEdadIngresados = SumEdadIngresados + Edad;
            PacIng ++;
            }
        else{
            SumEdadOperados = SumEdadOperados + Edad;
            PacOp ++;
            }
        Pacientes --;
        }
    
    if(PacAn == 0) {PacAn = 1;}
    if(PacIng == 0) {PacIng = 1;}
    if(PacOp == 0) {PacOp = 1;}
    printf("Edad promedio Pacientes Analizados: %d \nEdad promedio Pacientes Ingresados: %d \nEdad promedio Pacientes Operados: %d \n", SumEdadAnalizados / PacAn, SumEdadIngresados / PacIng, SumEdadOperados / PacOp);
    system("Pause");
    return 0;
}