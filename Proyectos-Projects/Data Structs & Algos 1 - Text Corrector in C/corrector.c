#include "corrector.h"

int max(int x, int y) {
    if (x > y) return x;
    return y;
}

Trie crear_trie_diccionario(char* dictFilename) {
    FILE* f = fopen(dictFilename, "r");
    if (f == NULL) {
        printf("Error. No se encontro el diccionario.\n");
        exit(1);
    }

    Trie trieDict = trie_crear();

    char buffer[256];
    char c;
    int i = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            buffer[i] = '\0';
            trie_insertar(&trieDict, (unsigned char*) buffer);
            i = 0;
        }
        else 
            buffer[i++] = c; 
    }
    fclose(f);
    // Insertamos la última palabra, en el caso en que no haya '\n' al final del archivo.
    if (i != 0)
        trie_insertar(&trieDict, (unsigned char*) buffer);
    return trieDict;
}

void verificar_incorrectas(Palabra** arrayPalabras, tablaHash incorrectasEncontradas, Trie trieDict, int len, int *arrSize, int* cantIncorrectas, char* str, int linea) {
    if (len <= 1 || trie_buscar(trieDict, (unsigned char*) str)) return;
    int idxEncontrada = tablaHash_buscar_index(incorrectasEncontradas, str);
    
    // Si la palabra ya había sido encontrada, solo se añade la línea en que se encontró.
    if (idxEncontrada != -1) {
        palabra_agregar_linea((*arrayPalabras)[idxEncontrada], linea);
        return;
    }
    tablahash_insertar(&incorrectasEncontradas, str, *cantIncorrectas,0);
    // Añadiendo palabra incorrecta
    if(*cantIncorrectas == *arrSize) {
        (*arrSize) *= 2;
        (*arrayPalabras) = realloc((*arrayPalabras), sizeof(Palabra) * (*arrSize));
    }
    (*arrayPalabras)[*cantIncorrectas] = palabra_crear(str, linea);
    (*cantIncorrectas)++;
}

Palabra* palabras_incorrectas(char* filename, int* cantIncorrectas, Trie trieDict) {
    FILE* f = fopen(filename, "r");
    assert(f != NULL);
    
    *cantIncorrectas = 0;
    int arrSize = 4; 
    Palabra* arrPalabrasIncorrectas = malloc(sizeof(Palabra)* arrSize);

    tablaHash incorrectasEncontradas = tablahash_crear(256, (FunHash) DJB2Hash);

    int lineaActual = 1, i = 0;
    char c;
    char buffer[256];
    while ((c = getc(f)) != EOF) {
        if (c == '\n') {
            buffer[i] = '\0';
            verificar_incorrectas(&arrPalabrasIncorrectas, incorrectasEncontradas, trieDict, i, &arrSize, cantIncorrectas, buffer, lineaActual);
            i = 0;
            lineaActual++;
        }
        // Si no es un caracter A-Z o a-z, se considera que la palabra terminó, y se verifica su validez. 
        else if ((c < 'A' || c > 'z')) {
            buffer[i] = '\0';
            verificar_incorrectas(&arrPalabrasIncorrectas, incorrectasEncontradas, trieDict, i, &arrSize, cantIncorrectas, buffer, lineaActual);
            i = 0;
        }
        else
            buffer[i++] = tolower(c);
    }
    // Agregamos la última palabra del archivo en caso de que termine en A-Z o a-z.
    if (i != 0) {
        buffer[i] = '\0';
        verificar_incorrectas(&arrPalabrasIncorrectas, incorrectasEncontradas, trieDict, i, &arrSize, cantIncorrectas, buffer, lineaActual);
    }
    fclose(f);
    tablahash_destruir(incorrectasEncontradas);
    return arrPalabrasIncorrectas;
}

void buscar_correcciones(Trie trieDict, Palabra incorrecta, tablaHash *encontradas, int lenStr, int idxMinMod, int idxMaxMod, int pasosRestantes) {
    // Verificamos si es correcto seguir iterando. 
    if (pasosRestantes == 0 || incorrecta->cantSugerencias == CANT_MAX_SUGERENCIAS) return;
    
    // Intercambiando letras adyacentes en la palabra.
    for (int idx = max(idxMinMod-1, 0); incorrecta->str[idx+1] != '\0' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && idx != idxMaxMod; idx++) {
        intercambiar_letras(&(incorrecta->str), idx);
        verificar_correccion_recursion(trieDict, incorrecta, encontradas, lenStr, idx, pasosRestantes);
        intercambiar_letras(&(incorrecta->str), idx);
    }
    // Modificando cada letra en la palabra.
    for (int idx = idxMinMod; incorrecta->str[idx] != '\0' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && idx != idxMaxMod; idx++) {
        char temp = incorrecta->str[idx];
        for (char letra = 'a'; letra <= 'z' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS; letra++) {
            modificar_letra(&(incorrecta->str), idx, letra);
            verificar_correccion_recursion(trieDict, incorrecta, encontradas, lenStr, idx, pasosRestantes);
            modificar_letra(&(incorrecta->str), idx, temp);
        }
    }
    // Eliminando cada letra en la palabra.
    for (int idx = idxMinMod; incorrecta->str[idx] != '\0' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && idx != idxMaxMod+MAX_CORRECCIONES-1; idx++) {
        char temp = incorrecta->str[idx];
        eliminar_letra(&(incorrecta->str), lenStr, idx);
        verificar_correccion_recursion(trieDict, incorrecta, encontradas, lenStr-1, idx, pasosRestantes);
        insertar_letra(&(incorrecta->str), lenStr-1, idx, temp);
    }
    // Insertando letras en cada posición de la palabra.
    for (int idx = idxMinMod; incorrecta->str[idx-1] != '\0' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && idx != idxMaxMod+1; idx++) {
        for (char letra = 'a'; letra <= 'z' && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS; letra++) {
            insertar_letra(&(incorrecta->str), lenStr, idx, letra);
            verificar_correccion_recursion(trieDict, incorrecta, encontradas, lenStr+1, idx, pasosRestantes);
            eliminar_letra(&(incorrecta->str), lenStr+1, idx);
        }
    }
}

void verificar_correccion_recursion(Trie trieDict, Palabra incorrecta, tablaHash *encontradas, int lenStr, int idxMinMod, int pasosRestantes) {
    int idxMaxMod = -MAX_CORRECCIONES;
    int repetida = tablaHash_buscar_filtrar_repetidas(*encontradas, incorrecta->str, &idxMaxMod, pasosRestantes);

    if (repetida != 1) {
        
        buscar_palabras_adyacentes(trieDict, incorrecta, &idxMaxMod, pasosRestantes-1, repetida);
        if (repetida == 0)
            tablahash_insertar(encontradas, incorrecta->str, idxMaxMod, pasosRestantes);

        if (idxMaxMod+1 >= idxMinMod || idxMaxMod == -MAX_CORRECCIONES)
            buscar_correcciones(trieDict, incorrecta, encontradas, lenStr, idxMinMod, idxMaxMod, pasosRestantes-1);
    }
}

char* separar_palabras(char* str, int indexSeparacion) {
    char* palabras = malloc(sizeof(char) * 256);
    strcpy(palabras, str);
    palabras[indexSeparacion] = ' ';
    palabras[indexSeparacion+1] = '\0'; 
    strcat(&(palabras[indexSeparacion+1]), &(str[indexSeparacion]));
    return palabras;
}

void buscar_palabras_adyacentes(Trie trieDict, Palabra incorrecta, int* idxMaxMod, int pasosRestantes, int repetida) {    
    Trie temp = trieDict;
    int idxMaxModAdy = 0; // Será el index máximo de modificación de la palabra adyacente en caso de existir una.

    for (int i = 0; incorrecta->str[i] != '\0'; i++) {
        if (temp->terminal) {
            if (buscar_palabra(trieDict, (unsigned char*)&(incorrecta->str[i]), &idxMaxModAdy)) {
                idxMaxModAdy = -1;
                if(incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && ((repetida == 0 && pasosRestantes > 0) || repetida == 3))
                    palabra_agregar_sugerencia(incorrecta, separar_palabras(incorrecta->str, i));
            }  
        }
        if (idxMaxModAdy > 0)
            idxMaxModAdy--; // Por cada letra que por la que iteramos consideramos que estamos más cerca del idxMaxModAdy.

        if (temp->children[incorrecta->str[i]-'a'] == NULL) {
            // Devolvemos el index donde el Trie termina para evitar modificar palabras donde ya es imposible encontrar correcciones.
            if (idxMaxModAdy >= 0)
                *idxMaxMod = i+idxMaxModAdy+1;
            return;
        }
        
        temp = temp->children[incorrecta->str[i]-'a'];
    }

    if (temp->terminal && incorrecta->cantSugerencias < CANT_MAX_SUGERENCIAS && repetida == 0) {
        palabra_agregar_sugerencia(incorrecta, incorrecta->str);
    }
    // Si se logró recorrer todo el Trie sin que se termine, permitiremos correcciones
    // sobre el largo total del string en la próxima aplicación de buscar_correcciones.
}

bool buscar_palabra(Trie trieDict, unsigned char* str, int* idxMaxModAdy) {
    Trie temp = trieDict;
    for (int i = 0; str[i] != '\0'; i++) {
        if(temp->children[str[i]-'a'] == NULL) {
            if (*idxMaxModAdy >= 0)
                *idxMaxModAdy = max(*idxMaxModAdy, i);
            return false;
        }
            
        temp = temp->children[str[i]-'a'];
    }
    return temp->terminal;
}

void intercambiar_letras(char** str, int index) {
    char temp = (*str)[index];
    (*str)[index] = (*str)[index+1];
    (*str)[index+1] = temp;
}

void modificar_letra(char** str, int index, char letra) {
    (*str)[index] = letra;
}

void insertar_letra(char** str, int len, int index, char letra) {
    memmove(&(*str)[index+1], &(*str)[index], len+1 - index);
    (*str)[index] = letra;
}

void eliminar_letra(char** str, int len, int index) {
    memmove(&(*str)[index], &(*str)[index + 1], len - index);
}

void corregir_palabras(Trie trieDict, Palabra* palabrasIncorrectas, int cantIncorrectas) {
    tablaHash encontradas = tablahash_crear(1024, (FunHash)DJB2Hash);

    // Iteramos sobre cada una de las palabras incorrectas encontradas.
    for (int i = 0; i < cantIncorrectas; i++) {
        int lenStr = strlen(palabrasIncorrectas[i]->str);
        int idxMaxMod = -MAX_CORRECCIONES;
        // Verificamos si hay palabras adyacentes en el string original antes de realizar modificaciones.
        buscar_palabras_adyacentes(trieDict, palabrasIncorrectas[i], &idxMaxMod, 1, 0);
        // Insertamos el string original en la tablahash para marcar que ya fue encontrada.
        tablahash_insertar(&encontradas, palabrasIncorrectas[i]->str, idxMaxMod, 0);
        // Verificamos si se encuentran correcciones en 1, 2, ..., MAX_CORRECCIONES pasos en orden.
        for (int j = 1; j <= MAX_CORRECCIONES && palabrasIncorrectas[i]->cantSugerencias < CANT_MAX_SUGERENCIAS;  j++) {
            buscar_correcciones(trieDict, palabrasIncorrectas[i], &encontradas, lenStr, 0, idxMaxMod, j);
        }
        // Vaciamos la tablahash sin eliminarla para reutilizar para corregir la próxima palabra.
        tablahash_vaciar(encontradas);
    }
    free(encontradas->elems);
    free(encontradas);
}

void escribir_archivo(Palabra* palabrasCorregidas, int cantIncorrectas, char* outputFileName) {
    FILE* f = fopen(outputFileName, "w");

    if (cantIncorrectas == 0) {
        fprintf(f, "No se hallaron palabras incorrectas.");
        fclose(f);
        return;
    }

    for (int i = 0; i < cantIncorrectas; i++) {
        fprintf(f, "Linea(s) ");
        for (int j = 0; j < palabrasCorregidas[i]->cantLineas; j++) {
            fprintf(f, "%d, ", palabrasCorregidas[i]->lineas[j]);
        }

        fprintf(f, "\"%s\" no esta en el diccionario.", strupper(&(palabrasCorregidas[i]->str)));

        if (palabrasCorregidas[i]->cantSugerencias != 0) {
            fprintf(f, "\nQuizas quiso decir: ");
            for (int j = 0; j < palabrasCorregidas[i]->cantSugerencias; j++)
                fprintf(f, " \"%s\"", strupper(&(palabrasCorregidas[i]->sugerencias[j])));

            fprintf(f, "\n");
        }
        else
            fprintf(f, "\nNo se encontraro correcciones.\n");
    }
    fclose(f);
}

char* strupper(char** str) {
    for (int i = 0; (*str)[i] != '\0'; i++) {
        (*str)[i] = toupper((unsigned char) (*str)[i]);
    }
    return *str;
}