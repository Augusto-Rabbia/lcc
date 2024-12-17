import random


with open("Caracteristicas.txt", "r") as archivoaLeer:
    palabras = []
    for i, linea in enumerate(archivoaLeer):
        if i == 1:
            dimension = int(linea.strip())
        elif i == 3:
            complejidad = int(linea.strip())
        elif i > 4:
            palabras.append(linea.strip().upper())

print(dimension)
print(complejidad)
print(palabras)

abecedario = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

sopaMatriz = [["_" for x in range(dimension)] for y in range(dimension)]


def horizontal_derecha(palabra, sopa):
    x = random.randint(0,dimension-len(palabra))
    y = random.randint(0,dimension-1)
    
    for i in range(len(palabra)):
        sopa[y][x+i] = palabra[i]

def horizontal_izquierda(palabra, sopa):
    return horizontal_derecha(palabra[::-1], sopa)

def vertical_abajo(palabra, sopa):
    x = random.randint(0,dimension-1)
    y = random.randint(0,dimension-len(palabra))
    
    for i in range(len(palabra)):
        sopa[y+i][x] = palabra[i]

def vertical_arriba(palabra, sopa):
    return vertical_abajo(palabra[::-1], sopa)

def diagonal_ab_der(palabra, sopa):
    x = random.randint(0,dimension-len(palabra))
    y = random.randint(0,dimension-len(palabra))
    
    for i in range(len(palabra)):
        sopa[y+i][x+i] = palabra[i]

def diagonal_arr_izq(palabra, sopa):
    return diagonal_ab_der(palabra[::-1], sopa)

def diagonal_arr_der(palabra, sopa):
    x = random.randint(0,dimension-len(palabra))
    y = random.randint(len(palabra)-1,dimension-1)
    
    for i in range(len(palabra)):
        sopa[y-i][x+i] = palabra[i]
    return sopa

def diagonal_ab_izq(palabra, sopa):
    return diagonal_arr_der(palabra[::-1], sopa)

#horizontal_derecha("derecha", sopaMatriz)
#horizontal_izquierda("izquierda", sopaMatriz)
#vertical_abajo("abajo", sopaMatriz)
#vertical_arriba("arriba", sopaMatriz)
#diagonal_ab_der("ab-der", sopaMatriz)
#diagonal_arr_izq("arr-izq", sopaMatriz)
#diagonal_arr_der("arr-der", sopaMatriz)
#diagonal_ab_izq("ab-izq", sopaMatriz)

def armar_complejidad_0(sopa):
    for i in palabras:
        direccion = random.randint(0, 3)
        if direccion == 0:
            horizontal_derecha(i, sopa)
        elif direccion == 1:
            horizontal_izquierda(i, sopa)
        elif direccion == 2:
            vertical_abajo(i, sopa)
        else:
            vertical_arriba(i, sopa)


def armar_complejidad_1(sopa):
    for i in palabras:
        direccion = random.randint(0, 4)
        if direccion == 0:
            horizontal_derecha(i, sopa)
        elif direccion == 1:
            horizontal_izquierda(i, sopa)
        elif direccion == 2:
            vertical_abajo(i, sopa)
        elif direccion == 3:
            vertical_arriba(i, sopa)
        else:
            diagonal_ab_der(i, sopa)

def armar_complejidad_2(sopa):
    pass

def armar_complejidad_3(sopa):
    for i in palabras:
        direccion = random.randint(0, 7)
        if direccion == 0:
            horizontal_derecha(i, sopa)
        elif direccion == 1:
            horizontal_izquierda(i, sopa)
        elif direccion == 2:
            vertical_abajo(i, sopa)
        elif direccion == 3:
            vertical_arriba(i, sopa)
        elif direccion == 4:
            diagonal_ab_der(i, sopa)
        elif direccion == 5:
            diagonal_arr_izq(i, sopa)
        elif direccion == 6:
            diagonal_arr_der(i, sopa)
        else:
            diagonal_ab_izq(i, sopa)

def juego(sopa, complejidad): 
    if complejidad == 0:
        armar_complejidad_0(sopa)
    elif complejidad == 1:
        pass
    elif complejidad == 2:
        pass
    else:
        armar_complejidad_3(sopa)
        

def main():
    # meter todo el resto del codigo aca
    juego(sopaMatriz, complejidad)

    for i in sopaMatriz:
        print(i)



main()

