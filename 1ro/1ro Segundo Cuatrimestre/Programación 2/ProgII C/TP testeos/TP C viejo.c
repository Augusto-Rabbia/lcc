#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_LINES 90000

#define MAX_WORD_LEN 23 //La palabra más larga reconocida por la Real Academia Española tiene 23 letras.

#define MIN_DIMENSION 4 //Si la sopa de letras fuera demasiado pequeña casi ninguna palabra podría entrar.

void pedir_datos(char *nomFILE, int *dim, int *cantP, int *complej){
    printf("Nombre del archivo a escanear: ");
    gets(nomFILE);
    printf("Tamaño de la sopa de letras: ");
    scanf("%d", dim);
    printf("Cantidad de palabras de la sopa de letras: ");
    scanf("%d", cantP);
    printf("Complejidad de la sopa de letras: ");
    scanf("%d", complej);
}

void chequear_datos(char *nomFILE, int dim, int cantP, int complej){
    int hayError;
    FILE *pArchivoaEscribir;
    if ((pArchivoaEscribir = fopen(nomFILE, "r"))){fclose(pArchivoaEscribir);}
    else{printf("Error: Archivo no encontrado.\n");
    hayError = 1;}
    if(dim < MIN_DIMENSION){printf("Error: El tamaño de la sopa de letras es inválido o demasiado bajo, intente con un entero mayor a 3.\n");
    hayError = 1;}
    if(cantP < 0){printf("Error: La cantidad de palabras debe ser un entero positivo.\n");
    hayError = 1;}
    if(complej < 1 || complej > 3){printf("Error: La complejidad debe ser un entero de 1 a 3, intente de nuevo.\n");
    hayError = 1;}
    if(hayError){
        printf("El programa se cerrará automáticamente...\n");
        sleep(5);
        exit;
    }
}
/*
void escribir_archivo(FILE *archivo, int dim, int cantP, int complej, int cantLineas, char Palabras){
    fprintf(archivo, "DIMENSION\n%d\nCOMPLEJIDAD\n%d\nPALABRAS\n", dim, complej);

    
    srand(time(NULL));
    int elegido, add, descartados[cantP]; //Elegido escogerá un número aleatoriamente y descartados se encargará de asegurarse que los números no se repitan

    for(int i = 0; i < cantP; i++){
        elegido = (rand()*rand()) % cantLineas; //Generando los números aleatorios para obtener las palabras. Nota: mi RAND_MAX está definido como 32767, así que tuve que buscar una manera de que obtenga números más altos.
        printf("%d\n", elegido);
        for(int j = 0; j < i; j++){
            if(elegido == descartados[j] || Palabras[elegido] > dim){add = 0;} 
            else{add = 1;}
            }

        if(add){
                fprintf(archivo, "%s", Palabras[elegido]); //Si la palabra no ha sido elegida aún y no es más grande que la sopa de letras, se escoge.
            }
        else{i--;}
    }
}*/

int main(){
    char *nomArchivo = malloc(sizeof(char)*25);
    int dimension, cantPalabras, complejidad;
    
    pedir_datos(nomArchivo, &dimension, &cantPalabras, &complejidad);
    chequear_datos(nomArchivo, dimension, cantPalabras, complejidad);

    FILE *pArchivoaLeer = fopen(nomArchivo, "r");
    
    int cantLineas = 0;
    char lineas[MAX_LINES][MAX_WORD_LEN], buffer[255];
    while(fgets(buffer, 255, pArchivoaLeer) != NULL){
        strcpy(lineas[cantLineas], buffer);
        cantLineas ++;
    }
    
    fclose(pArchivoaLeer);

    FILE *pArchivoaEscribir = fopen("Caracteristicas.txt", "w");

    //escribir_archivo(pArchivoaEscribir, dimension, cantPalabras, complejidad, cantLineas, *lineas);

    fprintf(pArchivoaEscribir, "DIMENSION\n%d\nCOMPLEJIDAD\n%d\nPALABRAS\n", dimension, complejidad);
    
    srand(time(NULL));
    int elegido, add, descartados[cantPalabras]; //Elegido escogerá un número aleatoriamente y descartados se encargará de asegurarse que los números no se repitan
    for(int i = 0; i < cantPalabras; i++){
        elegido = (rand()*rand()) % cantLineas; //Generando los números aleatorios para obtener las palabras. Nota: mi RAND_MAX está definido como 32767, así que tuve que buscar una manera de que obtenga números más altos.
        printf("%d\n", elegido);
        for(int j = 0; j < i; j++){
            if(elegido == descartados[j]){add = 0;}
            else{add = 1;}
            }

        if(add && strlen(lineas[elegido]) <= dimension + 1){
                fprintf(pArchivoaEscribir, "%s", lineas[elegido]); //Si la palabra no ha sido elegida aún y no es más grande que la sopa de letras, se escoge.
            }
        else{i--;}
    }

    fclose(pArchivoaEscribir);
    system("Pause");
    return 0;
}









