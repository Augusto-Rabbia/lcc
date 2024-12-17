#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





int main(){
    FILE *pArchivoaLeer = fopen("testeos.txt", "r");
    
    
    int cantLineas;
    char lineas[100000][25], buffer[255];
    while(fgets(buffer, 255, pArchivoaLeer) != NULL){
        strcpy(lineas[cantLineas], buffer);
        cantLineas ++;
    }
    
    fclose(pArchivoaLeer);
    system("Pause");
    return 0;
}