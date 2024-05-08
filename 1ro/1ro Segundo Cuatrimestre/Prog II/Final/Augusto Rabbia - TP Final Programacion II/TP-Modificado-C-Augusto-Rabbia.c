#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MIN_DIMENSION 3 //Definido el valor arbitrario considerado la mínima dimension permitida para la sopa.

//pedir_datos: String, Int, Int, Int -> void
//Obtiene el input del usuario. Este input constituye las características de la sopa de letras.
void pedir_datos(char *nomFILE, int *dim, int *cantP, int *complej) {
    printf("Nombre del archivo a escanear: ");
    gets(nomFILE);
    printf("Tamaño de la sopa de letras: ");
    scanf("%d", dim);
    printf("Cantidad de palabras de la sopa de letras: ");
    scanf("%d", cantP);
    printf("Complejidad de la sopa de letras: ");
    scanf("%d", complej);
}

//chequear_datos: String, Int, Int, Int -> void
//verifica que los datos que fueron dados por el usuario sean válidos.
//Cada error es comunicado al usuario individualmente. Si se haya uno de estos errores, el programa se cierra.
void chequear_datos(char *nomFILE, int dim, int cantP, int complej) {
    int hayError;
    FILE *pArchivoaEscribir;
    if ((pArchivoaEscribir = fopen(nomFILE, "r"))) fclose(pArchivoaEscribir);
    else {
        printf("Error: Archivo no encontrado.\n");
        hayError = 1;
    }
    if (dim < MIN_DIMENSION) {
        printf("Error: El tamaño de la sopa de letras debe ser un entero mayor a 2.\n");
        hayError = 1;
    }
    if (cantP < 0) {
        printf("Error: La cantidad de palabras debe ser un entero positivo.\n");
        hayError = 1;
    }
    if (complej < 0 || complej > 3) {
        printf("Error: La complejidad debe ser un entero de 0 a 3, intente de nuevo.\n");
        hayError = 1;
    }
    if (hayError) {
        system("Pause");
        exit(0);
    }
}

//palabras_elegidas: FILE, Int, Int, Int, Int -> Array{String}
//Se seleccionan aleatoriamente las palabras del archivo de ingreso.
//contador: cuenta cuántas palabras válidas se han encontrado con el fin de asegurarse que todas estas tengan la misma posibilidad de ser seleccionadas.
char **palabras_elegidas(FILE *archivoLeer, int *palabrasH, int cantPalabras, int dimension) {
    int contador = 0, elegir, posReemplazada, palabrasHalladas = 0;
    char buffer[255], *palabras_aux, **palabras;
    palabras_aux = malloc(sizeof(char*)*cantPalabras);
    for(int i = 0; i < cantPalabras; i++) {
        palabras[i] = realloc(palabras_aux[i], sizeof(char)*255);
    }

    while(fgets(buffer, 255, archivoLeer) != NULL) {
        if (strlen(buffer) > dimension + 1 || strlen(buffer) <= 2) { //Se verifica que las palabras seleccionadas entren en la sopa de letras y no sean de un caracter.
            contador --;
        }
        else if (contador < cantPalabras) {
            strcpy(palabras[contador], buffer); //Se llena el array con las primeras (cantPalabras) palabras.
            palabrasHalladas++;
        }
        else {
            elegir = (rand() * rand()) % (contador * 100 / cantPalabras); //Se randomiza si se seleccionará la palabra. Nota: mi RAND_MAX está definido como 32767, por lo que utilicé este método para obtener números mayores.
            if (elegir <= 100) {
                posReemplazada = rand() % cantPalabras; //Se randomiza la posición en la sopa de la palabra si fue seleccionada para ser colocada en el array.
                strcpy(palabras[posReemplazada], buffer);
            }
        }
        
        contador ++;
    }
    *palabrasH = palabrasHalladas;
    return palabras;
}

//escribir_archivo: FILE, Array{String}, Int, Int, Int, Int -> void
//Crea, a partir de todos los datos dados por el usuario, un archivo llamado Caracteristicas.txt con los datos necesarios
//para la creacion de la sopa.
void escribir_archivo(FILE *archivoEscribir, char **palabras, int palabrasHalladas, int cantPalabras, int dimension, int complejidad){
    fprintf(archivoEscribir, "DIMENSION\n%d\nCOMPLEJIDAD\n%d\nPALABRAS\n", dimension, complejidad);

    if (palabrasHalladas != cantPalabras) {
            printf("Error:No se han hallado en el archivo la cantidad de palabras que buscó que entren en la sopa de letras.\nSe ha creado el archivo con %d palabras.\n", palabrasHalladas);
    } //Solucion de posibles problemas.
    for (int i = 0; i != palabrasHalladas; i++) {
        fprintf(archivoEscribir, "%s", palabras[i]); //Se escriben los strings del array final en el archivo.
    }
}

int main() {
    srand(time(NULL));
    char nomArchivo[25];
    int dimension, cantPalabras, complejidad;
    
    pedir_datos(nomArchivo, &dimension, &cantPalabras, &complejidad);
    
    chequear_datos(nomArchivo, dimension, cantPalabras, complejidad);

    FILE *pArchivoaLeer = fopen(nomArchivo, "r");
    FILE *pArchivoaEscribir = fopen("Caracteristicas.txt", "w");

    int palabrasHalladas;
    char **elegidas;
    elegidas = palabras_elegidas(pArchivoaLeer, &palabrasHalladas, cantPalabras, dimension);

    escribir_archivo(pArchivoaEscribir, elegidas, palabrasHalladas, cantPalabras, dimension, complejidad);

    fclose(pArchivoaLeer);
    fclose(pArchivoaEscribir);

    free(elegidas);
    system("Pause");
    return 0;
}
