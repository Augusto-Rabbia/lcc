import random
import string


def leer_archivo():
    """leer_archivo: Null -> int, int, List(string)   \n
    obtiene los datos del archivo utilizado como Input."""
    with open("Caracteristicas.txt", "r") as archivoaLeer:
        palabras = []
        for i, linea in enumerate(archivoaLeer):
            if i == 1:
                dimension = int(linea.strip())
            elif i == 3:
                complejidad = int(linea.strip())
            elif i > 4:
                palabras.append(linea.strip().upper())
        return dimension, complejidad, palabras

def demasiadas_palabras(palabras, dimension):
    """demasiadas_palabras: List(string), int -> bool   \n
    Determina si la sopa es posible de crear verificando que la suma de los caracteres de las palabras es   \n
    menor que la cantidad de lugares en la sopa."""
    letras_totales = 0

    for palabra in palabras:
        letras_totales += len(palabra)
    if letras_totales > dimension*dimension:
        return True
    else: return False

def crear_sopa_vacia(dimension):
    """crear_sopa_vacia: int -> List(List(bool))   \n
    Crea una lista de listas (lista 2D o matriz) del tamaño adecuado, rellenada con False."""
    sopaMatriz = [[False for x in range(dimension)] for y in range(dimension)]
    return sopaMatriz

def horizontal(palabra, x, y, al_reves):
    """horizontal: string, int, int, bool -> List(Tuple(int, int, char))    \n
    Devuelve las posiciones donde se colocaran cada una de las letras de una palabra de manera horizontal.  \n
    Si al_reves es verdadero, invierte la palabra. """
    if (al_reves): palabra = palabra[::-1]
    posiciones = []
    for i in range(len(palabra)):
        posiciones.append((x+i, y, palabra[i]))
    return posiciones

def vertical(palabra, x, y, al_reves):
    """vertical: string, int, int, bool -> List(Tuple(int, int, char))   \n
    Devuelve las posiciones donde se colocaran cada una de las letras de una palabra de manera horizontal.  \n
    Si al_reves es verdadero, invierte la palabra."""
    if (al_reves): palabra = palabra[::-1]
    posiciones = []
    for i in range(len(palabra)):
        posiciones.append((x, y+i, palabra[i]))
    return posiciones

def diagonal_principal(palabra, x, y, al_reves):
    """diagonal_principal: string, int, int, bool -> List(Tuple(int, int, char))   \n
    Devuelve las posiciones donde se colocaran cada una de las letras de una palabra de manera diagonal, sobre la diagonal principal.  \n
    Si al_reves es verdadero, invierte la palabra."""
    if (al_reves): palabra = palabra[::-1]
    posiciones = []
    for i in range(len(palabra)):
        posiciones.append((x+i, y+i, palabra[i]))
    return posiciones

def diagonal_secundaria(palabra, x, y, al_reves):
    """diagonal_secundaria: string, int, int, bool -> List(Tuple(int, int, char))   \n
    Devuelve las posiciones donde se colocaran cada una de las letras de una palabra de manera diagonal, sobre la diagonal secundaria.  \n
    Si al_reves es verdadero, invierte la palabra."""
    if (al_reves): palabra = palabra[::-1]
    posiciones = []
    for i in range(len(palabra)):
        posiciones.append((x+i, y-i, palabra[i]))
    return posiciones

def hayColision(sol, direcciones_sol, palabras, k, pueden_cruzar):
    """List(tuple(int, int)), List(int), List(string), int, bool    \n
    Determina si una palabra siendo colocada en una posición inválida, porque está chocando con otra    \n
    palabra, con la que no comparte esta letra (si pueden_cruzar es verdadero)."""
    funciones_direccion =[lambda palabra, x, y: horizontal(palabra, x, y, False),
                lambda palabra, x, y: vertical(palabra, x, y, False),
                lambda palabra, x, y: diagonal_secundaria(palabra, x, y, False),
                lambda palabra, x, y: diagonal_principal(palabra, x, y, False)]

    posiciones_palabra_actual = (funciones_direccion[direcciones_sol[k] % 4])(palabras[k], sol[k][0], sol[k][1])
    # Tuplas formato (x, y, letra)

    i = 0
    colisiona = False
    while i < k and not colisiona:
        # Temp es una lista de tuplas, de todas las posiciones que ocupa la palabra 'palabras[i]'
        temp = (funciones_direccion[direcciones_sol[i] % 4])(palabras[i], sol[i][0], sol[i][1]) 

        for posicion in temp:
            for posicion_k in posiciones_palabra_actual:
                if (posicion[0] == posicion_k[0] and posicion[1] == posicion_k[1]):
                    if pueden_cruzar:
                        colisiona = posicion[2] != posicion_k[2]
                    else:
                        colisiona = True
                if (colisiona): break

            if (colisiona): break
        i += 1

    return colisiona

def esValida(sol, direcciones_sol, direcciones, palabras, dimension_sopa, k, pueden_cruzar):
    """esValida: List(tuple(int, int)), List(int), int, int, bool   \n
    Asigna una dirección a la última palabra, probando si entra en cada dirección posible, de manera aleatoria."""
    direcciones_posibles = direcciones.copy()
    random.shuffle(direcciones_posibles) # Mezclamos las posiciones para que sea aleatorio
    found = False # True si luego encontramos una posición válida
    while len(direcciones_posibles) > 0 and not found:
        dir = direcciones_posibles.pop()
        if (dir % 4 == 0):
            found = sol[k][0] <= dimension_sopa-len(palabras[k])
        elif (dir % 4 == 1):
            found = sol[k][1] <= dimension_sopa-len(palabras[k])
        elif (dir % 4 == 2):
            found = sol[k][0] <= dimension_sopa-len(palabras[k]) and sol[k][1] >= len(palabras[k])-1 and sol[k][1] <= dimension_sopa-1
        elif (dir % 4 == 3):
            found = sol[k][0] <= dimension_sopa-len(palabras[k]) and sol[k][1] <= dimension_sopa-len(palabras[k])
        if (found):
            direcciones_sol[k] = dir
            found = not hayColision(sol, direcciones_sol, palabras, k, pueden_cruzar)

    if (not found): direcciones_sol[k] = None

    return found

def backtrack(sol, direcciones_sol, direcciones_posibles, palabras, posiciones_disponibles, k, solucion_encontrada, dimension_sopa, pueden_cruzar):
    """backtrack: List(tuple(int, int)), List(int), List(string), int, List(bool), int, bool - > Null
    sol: List(tuple(int, int)). Donde las tuplas son una posición (x,y), donde empieza cada palabra  \n
    direcciones_sol: List(int). Direcciones a las que apunta cada elemento de la lista sol  \n
    palabras: List(string). Lista de palabras a colocar en la sopa  \n
    posiciones_disponibles: List(tuple(int, int)). Lista de todas las posiciones disponibles  \n
    k: int. La posición de 'sol' a la que estamos probando asignar un valor  \n
    solucion_encontrada: List(bool). Lista de un elemento de tipo bool, para parar el backtracking  \n
    dimension_sopa: int  \n
    pueden_cruzar: bool. Usado para verificar si una palabra puede cruzarse con otra"""
    i = 0
    while i < len(posiciones_disponibles) and not solucion_encontrada[0]:
        # Colocamos la palabra en una posicion (i,j) aleatoria
        pos = posiciones_disponibles[i]
        sol[k] = pos
        
        if esValida(sol, direcciones_sol, direcciones_posibles, palabras, dimension_sopa, k, pueden_cruzar):
            if (k == len(sol) - 1):
                solucion_encontrada[0] = True
            else:
                backtrack(sol, direcciones_sol, direcciones_posibles, palabras, posiciones_disponibles, k + 1, solucion_encontrada, dimension_sopa, pueden_cruzar)
        i += 1

def armar_sopa(sopa, dimension, complejidad, palabras):
    """armar_sopa: List(List(string)), int, int, List(string) -> Null   \n
    Ejecuta las funciones necesarias para colocar todas las palabras en la sopa de letras.
    """
    pueden_cruzar = False
    if complejidad == 0:
        opciones = 1
    elif complejidad == 1:
        opciones = 2
    else: 
        opciones = 7
        if complejidad == 3: pueden_cruzar = True

    # Creamos lista de tuplas con todas las posiciones posibles en las que puede comenzar una palabra
    lista_posiciones_aleatorias = []

    for i in range(dimension):
        for j in range(dimension):
            lista_posiciones_aleatorias.append((i, j))

    random.shuffle(lista_posiciones_aleatorias)
    sol = [None for x in range(len(palabras))]
    direcciones = [None for x in range(len(palabras))]
    direcciones_posibles = [x for x in range(opciones + 1)]
    solucion_encontrada = [False]
    
    backtrack(sol, direcciones, direcciones_posibles, palabras, lista_posiciones_aleatorias, 0, solucion_encontrada, dimension, pueden_cruzar)

    funciones_direccion = [lambda palabra, x, y, al_reves: horizontal(palabra, x, y, al_reves),
                lambda palabra, x, y, al_reves: vertical(palabra, x, y, al_reves),
                lambda palabra, x, y, al_reves: diagonal_secundaria(palabra, x, y, al_reves),
                lambda palabra, x, y, al_reves: diagonal_principal(palabra, x, y, al_reves)]

    if (solucion_encontrada[0]):
        for (i,pos) in enumerate(sol):
            temp = (funciones_direccion[direcciones[i] % 4])(palabras[i], pos[0], pos[1], direcciones[i] >= 4)
            for pos in temp:
                sopa[pos[1]][pos[0]] = pos[2]
    else:
        print("No es posible crear la sopa de letras.")

def verificarRepeticiones(sopa, lista_palabras, lista_direccion, deltaX, deltaY):
    """verificarRepeticiones: list(list(string)), list(String), list(String), Int, Int -> Null   \n
    Modifica las lineas verticales u horizontales de la sopa donde encuentre una solución y modifica las letras    \n
    minúsculas (que fueron colocadas durante el rellenado) que podrían formar a esta palabra"""
    terminado = True
    for palabra in lista_palabras:
        y = 0
        x = 0
        contador = 0
        for i in lista_direccion:

            if palabra in i.upper() or palabra[::-1] in i.upper():
                for letra in i:

                    if letra.islower() and sopa[y][x].upper() in palabra and sopa[y][x] not in palabra:
                        nueva_letra = string.ascii_lowercase.replace(letra, "")[random.randint(0, len(string.ascii_lowercase)-2)]
                        sopa[y][x] = nueva_letra
                        lista_direccion[contador] = lista_direccion[contador].replace(letra, nueva_letra)
                        terminado = False

                    y += deltaX
                    x += deltaY

            if(deltaX == 1):
                y = 0
            else: x = 0        
            y += deltaY
            x += deltaX
            contador += 1

    return terminado

def verificarRepeticiones_diag_princ(sopa, dimension_sopa, lista_palabras, lista_direccion):
    """verificarRepeticiones_diag_princ: list(list(string)), int, list(String), list(String)-> Null   \n
    Modifica las lineas de la diagonal principal de la sopa donde encuentre una solución y modifica las letras    \n
    minúsculas (que fueron colocadas durante el rellenado) que podrían formar a esta palabra"""
    terminado = True
    for palabra in lista_palabras:
        y = dimension_sopa - 1
        x = 0
        contador = 0

        for i in lista_direccion:

            if palabra in i.upper() or palabra[::-1] in i.upper():
                for letra in i:

                    if letra.islower() and sopa[y][x].upper() in palabra and sopa[y][x] not in palabra:
                        nueva_letra = string.ascii_lowercase.replace(letra, "")[random.randint(0, len(string.ascii_lowercase)-2)]
                        sopa[y][x] = nueva_letra
                        lista_direccion[contador] = lista_direccion[contador].replace(letra, nueva_letra)
                        terminado = False
                    y += 1
                    x += 1

            contador += 1
            if(dimension_sopa > contador):
                y = dimension_sopa - contador - 1
                x = 0
            else: 
                x = 1 + contador - dimension_sopa
                y = 0
    return terminado

def verificarRepeticiones_diag_sec(sopa, dimension_sopa, lista_palabras, lista_direccion):
    """verificarRepeticiones_diag_sec: list(list(string)), int, list(String), list(String)-> Null   \n
    Modifica las lineas de la diagonal secundaria de la sopa donde encuentre una solución y modifica las letras    \n
    minúsculas (que fueron colocadas durante el rellenado) que podrían formar a esta palabra"""
    terminado = True
    for palabra in lista_palabras:
        y = 0
        x = 0
        contador = 0
        for i in lista_direccion:

            if palabra in i.upper() or palabra[::-1] in i.upper():
                for letra in i:
                    if letra.islower() and sopa[y][x].upper() in palabra and sopa[y][x] not in palabra:
                        nueva_letra = string.ascii_lowercase.replace(letra, "")[random.randint(0, len(string.ascii_lowercase)-2)]
                        sopa[y][x] = nueva_letra
                        lista_direccion[contador] = lista_direccion[contador].replace(letra, nueva_letra)
                        terminado = False
                    y -= 1
                    x += 1

            contador += 1
            if(dimension_sopa > contador):
                y = contador
                x = 0
            else: 
                y = dimension_sopa - 1
                x = contador - dimension_sopa + 1

    return terminado

def rellenar(sopa, dimension_sopa, palabras):
    """rellenar: list(list(string)), Int, list(String) -> Null   \n
    Coloca caracteres en minuscula en todos los lugares vacios de la sopa de letras y   \n
    obtiene las lineas horizontales, verticales y diagonales para dar como dato a las funciones    \n
    que verifican que este rellenado no cree palabras repetidas."""

    for x in range(dimension_sopa):
        for y in range(dimension_sopa):
            if sopa[y][x] == False:
                sopa[y][x] = string.ascii_lowercase[random.randint(0,len(string.ascii_lowercase)-1)]

    sin_errores = 0
    while(sin_errores != 4):
        sin_errores = 0
        cols = ["" for _ in range(dimension_sopa)]
        filas = ["" for _ in range(dimension_sopa)]
        diag_secundaria = ["" for _ in range(dimension_sopa*2 - 1)]
        diag_principal = ["" for _ in range(len(diag_secundaria))]

        for x in range(dimension_sopa):
            for y in range(dimension_sopa):
                cols[x] += (sopa[y][x])
        if verificarRepeticiones(sopa, palabras, cols, 1, 0): sin_errores += 1

        for x in range(dimension_sopa):
            for y in range(dimension_sopa):
                filas[y] += (sopa[y][x])
        if verificarRepeticiones(sopa, palabras, filas, 0, 1): sin_errores += 1

        for x in range(dimension_sopa):
            for y in range(dimension_sopa):
                diag_principal[x-y+dimension_sopa-1] += (sopa[y][x])
        if verificarRepeticiones_diag_princ(sopa, dimension_sopa, palabras, diag_principal): sin_errores += 1

        for x in range(dimension_sopa):
            for y in range(dimension_sopa):
                diag_secundaria[x+y] += (sopa[y][x])
        if verificarRepeticiones_diag_sec(sopa, dimension_sopa, palabras, diag_secundaria): sin_errores += 1

def juego(sopa, dimension, complejidad, palabras):
    """juego: List(List(string)), int, int, List(string) -> Null \n
    Ejecuta las funciones principales del programa"""
    if demasiadas_palabras(palabras, dimension):
        print("No es posible crear la sopa de letras.")
    else:
        armar_sopa(sopa, dimension, complejidad, palabras)
        rellenar(sopa, dimension, palabras)

def imprimir_resultados(sopa, palabras):
    """imprimir_resultado: List(List(string)), List(string) -> Null   \n
    Imprime la sopa de letras además de la lista de palabras para que puedan ser vistas por el usuario."""
    print("Sopa de Letras")
    resultado = ""
    for i in sopa:
       for j in i:
           resultado += j + " "
       print(resultado.upper())
       resultado = ""
    
    for i in palabras:
        resultado += i + " "
    print("\nPalabras: ", resultado)

def test_demasiadas_palabras():
    assert demasiadas_palabras(["HOLA", "TEST", "TODO", "SOLO", "COMO"], 4) == True
    assert demasiadas_palabras(["HOLA", "TEST"], 4) == False

def test_crear_sopa_vacia():
    assert crear_sopa_vacia(3) == [[False, False, False],[False, False, False],[False, False, False]]

def test_horizontal():
    assert horizontal("HOLA", 0, 0, False) == [(0, 0, 'H'), (1, 0, 'O'), (2, 0, 'L'), (3, 0, 'A')]
    assert horizontal("HOLA", 2, 5, True) == [(2, 5, 'A'), (3, 5, 'L'), (4, 5, 'O'), (5, 5, 'H')]

def test_vertical():
    assert vertical("HOLA", 0, 1, False) == [(0, 1, 'H'), (0, 2, 'O'), (0, 3, 'L'), (0, 4, 'A')]
    assert vertical("CHAU", 0, 0, True) == [(0, 0, 'U'), (0, 1, 'A'), (0, 2, 'H'), (0, 3, 'C')]

def test_diagonal_principal():
    assert diagonal_principal("HOLA", 0, 0, False) == [(0, 0, 'H'), (1, 1, 'O'), (2, 2, 'L'), (3, 3, 'A')]
    assert diagonal_principal("TEST", 3, 5, True) == [(3, 5, 'T'), (4, 6, 'S'), (5, 7, 'E'), (6, 8, 'T')]

def test_diagonal_secundaria():
    assert diagonal_secundaria("HOLA", 0, 5, False) == [(0, 5, 'H'), (1, 4, 'O'), (2, 3, 'L'), (3, 2, 'A')]
    assert diagonal_secundaria("HOLA", 1, 3, True) == [(1, 3, 'A'), (2, 2, 'L'), (3, 1, 'O'), (4, 0, 'H')]

def test_hayColision():
    assert hayColision([(9, 0), (1, 0), (1, 0)], [5, 5, 4], ['SALOMONICO', 'LAMENTABLE', 'BIOGRAFO'], 2, True) == True
    assert hayColision([(9, 0), (1, 0), (0, 1)], [5, 5, 1], ['SALOMONICO', 'LAMENTABLE', 'BIOGRAFO'], 2, False) == False

def test_verificarRepeticiones():
    sopa_fallo = [["C", "o", "n", "s"], ["O", "S", "E", "n"], ["N", 'c', 'o', 'n'], ['s', 'z', 'x', 'y']]
    sopa_pasa = [['C', 'z', 'z', 'z'], ['O', 'S', 'E', 'y'], ['N', 'y', 'y', 'y'], ['w', 'z', 'w', 'y']]

    cols = ["" for _ in range(4)]
    filas = ["" for _ in range(4)]
    for x in range(4):
            for y in range(4):
                cols[x] += (sopa_pasa[y][x])
    for x in range(4):
            for y in range(4):
                filas[y] += (sopa_fallo[y][x])

    assert verificarRepeticiones(sopa_fallo, ["CON", "ESO"], filas, 1, 0) == False
    assert verificarRepeticiones(sopa_pasa, ["CON", "ESO"], cols, 0, 1) == True

def test_verificarRepeticiones_diag_princ():
    sopa_fallo = [["C", "z", "z", "E"], ["O", "o", "z", "S"], ["N", 'z', 'n', 'O'], ['z', 'z', 'z', 'z']]
    sopa_pasa = [['C', 'z', 'z', 'z'], ['O', 'S', 'E', 'y'], ['N', 'y', 'y', 'y'], ['w', 'z', 'w', 'y']]

    diag_principal = ["" for _ in range(4*2 - 1)]
    for x in range(4):
            for y in range(4):
                diag_principal[x-y+4-1] += (sopa_fallo[y][x])
    assert verificarRepeticiones_diag_princ(sopa_fallo, 4, ["CON", "ESO"], diag_principal) == False

    diag_principal = ["" for _ in range(4*2 - 1)]
    for x in range(4):
            for y in range(4):
                diag_principal[x-y+4-1] += (sopa_pasa[y][x])
    assert verificarRepeticiones_diag_princ(sopa_pasa, 4, ["CON", "ESO"], diag_principal) == True

def test_verificarRepeticiones_diag_sec():
    sopa_fallo = [["C", "z", "z", "c"], ["O", "z", "o", "E"], ["N", 'n', 'z', 'S'], ['z', 'z', 'z', 'O']]
    sopa_pasa = [['C', 'z', 'z', 'z'], ['O', 'S', 'E', 'y'], ['N', 'y', 'y', 'y'], ['w', 'z', 'w', 'y']]

    diag_secundaria = ["" for _ in range(4*2 - 1)]
    for x in range(4):
            for y in range(4):
                diag_secundaria[x+y] += (sopa_fallo[y][x])
    assert verificarRepeticiones_diag_sec(sopa_fallo, 4, ["CON", "ESO"], diag_secundaria) == False

    diag_secundaria = ["" for _ in range(4*2 - 1)]
    for x in range(4):
            for y in range(4):
                diag_secundaria[x+y] += (sopa_pasa[y][x])
    assert verificarRepeticiones_diag_sec(sopa_pasa, 4, ["CON", "ESO"], diag_secundaria) == True

# Nota: esValida no es testeada por tener un componente random.

def main():
    dimension, complejidad, palabras = leer_archivo()
    sopaMatriz = crear_sopa_vacia(dimension)

    # Ordenamos las palabras de más larga a más corta para que el algoritmo sea más rápido
    palabras.sort(key=len, reverse=True)

    juego(sopaMatriz, dimension, complejidad, palabras)
    
    imprimir_resultados(sopaMatriz, palabras)

main()