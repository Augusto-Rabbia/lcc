def leer_archivo():
    """leer_archivo: None -> int, List(string)\n
    Obtiene los datos del tablero del archivo."""
    with open("Cuatro_en_Linea.txt", "r") as archivoALeer:
        index = 0
        tablero = []

        for line in archivoALeer.readlines():
            if index == 1:
                dimension = int(line)
            if index > 2:
                tablero.append(line.strip())
            index += 1

        return (dimension, tablero)

def fichas_flotantes(tablero, dimension):
    """fichas_flotantes: list[string], int -> bool\n
    Determina si hay o no fichas por encima de espacios vacíos."""

    encontrado = False

    for row in range(dimension-1, -1, -1):
        if not encontrado:
            for col in range(dimension-2, -1, -1):
                if tablero[row][col] == '_' and not encontrado:
                    for i in range(row):
                        if tablero[i][col] != '_':
                            encontrado = True
    return encontrado

def test_fichas_flotantes():
    assert fichas_flotantes(["1001",
                             "1000",
                             "__1_",
                             "001_",
                             "110_"], 5) == True

    assert fichas_flotantes(["_00_",
                             "_00_",
                             "_11_",
                             "0010",
                             "1100"], 5) == False

    assert fichas_flotantes(["_00_0",
                             "_0001",
                             "111_1",
                             "00110",
                             "110_0",
                             "01001"], 5) == True

def diferente_cantidad_fichas(tablero):
    """diferente_cantidad_fichas: list[string] -> bool\n
    Determina si la diferencia en cantidad de fichas\n
    de cada jugador es válida """
    cant_0 = 0
    cant_1 = 0
    for row in tablero:
        for ficha in row:
            if ficha == "0":
                cant_0 += 1
            elif ficha == "1":
                cant_1 += 1
    if(cant_1 < cant_0 - 1 or cant_0 < cant_1 - 1):
        return True
    return False 

def test_diferente_cantidad_fichas():
    assert diferente_cantidad_fichas([
                             "1001",
                             "1000",
                             "__1_",
                             "001_",
                             "1100"]) == True
    assert diferente_cantidad_fichas([
                             "1001",
                             "1011",
                             "__1_",
                             "1111",
                             "1111"]) == True
    assert diferente_cantidad_fichas([
                             "____",
                             "____",
                             "____",
                             "____",
                             "____"]) == False

def chequear_horizontal(tablero, numero):
    """chequear_horizontal: list[string], int -> bool\n
    Determina si hay 4 o más fichas consecutivas\n
    del mismo tipo en alguna horizontal"""
    encontrado = False
    for row in tablero:
        numSeguidos = 0
        for ficha in row:
            if ficha == numero:
                numSeguidos += 1
            else:
                numSeguidos = 0
            if numSeguidos == 4:
                encontrado = True
    return encontrado       

def test_chequear_horizontal():
    assert chequear_horizontal([
                             "1001",
                             "1011",
                             "__1_",
                             "1111",
                             "1111"], "1") == True
    assert chequear_horizontal([
                             "1001",
                             "1011",
                             "__1_",
                             "1111",
                             "1111"], "0") == False

    assert chequear_horizontal([
                             "10010",
                             "10111",
                             "_1__1",
                             "11110",
                             "11101",
                             "10110"], "1") == True
                             
    assert chequear_horizontal([
                             "1001",
                             "1011",
                             "__1_",
                             "1011",
                             "1101"], "0") == False

def chequear_vertical(dimension, tablero, numero):
    """chequear_vertical: int, list[string] -> bool\n
    determina si hay 4 o más fichas consecutivas\n
    del mismo tipo en alguna vertical"""
    encontrado = False
    for col in range(dimension-1):
        numSeguidos = 0
        if not encontrado:
            for row in range(dimension):
                
                if tablero[row][col] == numero:
                    numSeguidos += 1
                else:
                    numSeguidos = 0
                if numSeguidos == 4:
                    encontrado = True
    return encontrado

def test_chequear_vertical():
    assert chequear_vertical(5, [
                             "1001",
                             "1011",
                             "__1_",
                             "1111",
                             "1111"], "1") == True
    assert chequear_vertical(5, [
                             "1001",
                             "1011",
                             "__1_",
                             "1111",
                             "1111"], "0") == False

    assert chequear_vertical(6, [
                             "10101",
                             "11110",
                             "___1_",
                             "10110",
                             "11011",
                             "10110"], "1") == True
                             
    assert chequear_vertical(5, [
                             "1001",
                             "1011",
                             "__1_",
                             "1011",
                             "1011"], "0") == False
                             
def diagonales_posibles(dimension, diag):
    """diagonales_posibles: int, int -> list[tuple(int,int)]\n
    Dada la dimensión del tablero, encuentra dónde es posible\n
    que se encuentren 4 fichas seguidas en diagonal.\n
    diag = 0 es la diagonal principal \n
    diag != 0 es la diagonal secundaria"""
    posiciones_iniciales = []
    if diag == 0:
        for row in range(dimension-3):
            for col in range(dimension-4):
                if (row == 0 or col == 0):
                    posiciones_iniciales.append((row,col))
    else:
        for row in range(dimension-3):
            for col in range(3-row, dimension-1):
                if (row == 0 or col == dimension-2):
                    posiciones_iniciales.append((row,col))
    return posiciones_iniciales

def test_diagonales_posibles():
    assert diagonales_posibles(5, 0) == [(0,0), (1,0)]

    assert diagonales_posibles(5, 1) == [(0, 3), (1, 3)]

    assert diagonales_posibles(7, 1) == [(0, 3), (0, 4), (0, 5), (1, 5), (2, 5), (3, 5)]

def chequear_diag_principal(dimension, tablero, numero):
    """chequear_diag_principal: int, list[string], int -> bool\n
    Determina si hay 4 o más fichas consecutivas\n
    iguales en alguna diagonal principal"""
    diagonales_a_chequear = diagonales_posibles(dimension, 0)

    encontrado = False

    for (row, col) in diagonales_a_chequear:
        numSeguidos = 0
        while (row < dimension and col < dimension-1 and not encontrado):
            if tablero[row][col] == numero:
                numSeguidos += 1
            else:
                numSeguidos = 0
            if numSeguidos == 4:
                encontrado = True
            row += 1
            col += 1
    return encontrado

def test_chequear_diag_principal():
    assert chequear_diag_principal(5, [
                             "1001",
                             "1111",
                             "__1_",
                             "1011",
                             "1101"], "1") == True
    assert chequear_diag_principal(5, [
                             "1001",
                             "1010",
                             "__1_",
                             "1011",
                             "0111"], "0") == False

    assert chequear_diag_principal(6, [
                             "10010",
                             "10111",
                             "10_10",
                             "11111",
                             "01100",
                             "01111"], "1") == True
                             
    assert chequear_diag_principal(5, [
                             "1001",
                             "1011",
                             "__1_",
                             "1011",
                             "1101"], "0") == False

def chequear_diag_secundaria(dimension, tablero, numero):
    """chequear_diag_secundaria: int, list[string], int -> bool\n
    Determina si hay 4 o más fichas consecutivas\n
    del mismo tipo en alguna diagonal secundaria"""
    diagonales_a_chequear = diagonales_posibles(dimension, 1)

    encontrado = False
    
    for (row, col) in diagonales_a_chequear:
        numSeguidos = 0
        while(row < dimension and col >= 0):
                if tablero[row][col] == numero:
                    numSeguidos += 1
                else:
                    numSeguidos = 0
                if numSeguidos == 4:
                    encontrado = True
                row += 1
                col -= 1
    return encontrado

def test_chequear_diag_secundaria():
    assert chequear_diag_secundaria(5, [
                             "1001",
                             "1111",
                             "__1_",
                             "1011",
                             "1101"], "1") == False
    assert chequear_diag_secundaria(5, [
                             "1010",
                             "1001",
                             "001_",
                             "0111",
                             "1111"], "0") == True

    assert chequear_diag_secundaria(6, [
                             "10010",
                             "10110",
                             "_1__1",
                             "11011",
                             "10100",
                             "01111"], "1") == True
                             
    assert chequear_diag_secundaria(5,[
                             "1001",
                             "1011",
                             "_11_",
                             "1011",
                             "1101"], "0") == False

def tablero_incompleto(tablero):
    """tablero_incompleto: list[string] -> bool\n
    Determina si existen lugares vacíos en el tablero."""
    encontrado = False
    for row in tablero:
        for ficha in row:
            if ficha == "_":
                encontrado = True
    return encontrado

def test_tablero_incompleto():
    assert tablero_incompleto([
                             "1001",
                             "1011",
                             "__1_",
                             "1011",
                             "1101"]) == True

    assert tablero_incompleto([
                             "100101",
                             "101110",
                             "011110",
                             "111110",
                             "111001",
                             "111110"]) == False
                             
    assert tablero_incompleto([
                             "100_",
                             "1011",
                             "0010",
                             "1011",
                             "1011"]) == True
    
def juego(dimension, tablero):
    """juego: int, list[string] -> None\n
    Determina si hay o no ganadores y cuántos hay.\n
    Si hay 2 ganadores, comunica el error.\n
    Si no hay ganadores, determina si hubo empate."""
    gana_0 = chequear_vertical(dimension, tablero, "0") \
        or chequear_horizontal(tablero, "0") or chequear_diag_principal(dimension, tablero, "0") \
        or chequear_diag_secundaria(dimension, tablero, "0")

    gana_1 = chequear_vertical(dimension, tablero, "1") \
        or chequear_horizontal(tablero, "1") or chequear_diag_principal(dimension, tablero, "1") \
        or chequear_diag_secundaria(dimension, tablero, "1")

    if gana_0 and gana_1:
        print("ERROR: Partida no válida.\nGanaron ambos jugadores.")
    elif gana_0:
        print("El ganador es el jugador 0!")
    elif gana_1:
        print("El ganador es el jugador 1!")
    elif tablero_incompleto(tablero):
        print("La partida debe continuar.")
    else:
        print("Empate!")

def main():
    dimension, tablero = leer_archivo()
    if fichas_flotantes(tablero, dimension):
        print("ERROR: Partida no válida.\nExisten fichas flotantes.")
    elif diferente_cantidad_fichas(tablero):
        print("ERROR: Partida no válida.\nHay demasiadas fichas de un tipo jugador comparado con el otro.")
    else:
        juego(dimension, tablero)

main()