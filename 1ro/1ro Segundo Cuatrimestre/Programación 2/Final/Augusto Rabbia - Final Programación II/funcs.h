#ifndef __funcs_h__
#define __funcs_h__

/*
 * pedir_input: String, Int, Int, Int -> void
 * Obtiene el input del usuario. Este input constituye la dimension del tablero.
 */
void pedir_input(int* dim);

/*
 * pedir_memoria_strarr: int -> void
 * Asigna la memoria requerida en un array de strings
 * de forma periódica cuando sea necesario.
 */
char** generar_tablero_vacio(int dim);

/*
 * liberar_strarr: *array[string], int -> void
 * Libera la memoria de un array de strings.
 */
void liberar_strarr(char** strarr, int len);

/*
 * crear_tablero: int -> array[string]
 * Coloca las fichas de manera aleatoria en cada linea
 * del tablero y lo devuelve.
 */
char** crear_tablero(int dim);

/*
 * crear_tablero: array[string], int -> void
 * Crea un archivo e imprime el output en él. Este output está 
 * constituido por la dimensión y el tablero.
 */
void imprimir_tablero(char** tablero, int dim);

#endif
