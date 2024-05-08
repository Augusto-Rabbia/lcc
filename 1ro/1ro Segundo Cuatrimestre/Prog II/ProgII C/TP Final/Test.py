def demasiadas_palabras(palabras, dimension):
    letras_totales = 0

    for palabra in palabras:
        for letra in palabra:
            letras_totales += 1
    if letras_totales > dimension*dimension:
        return True
    else: return False

def test_demasiadas_palabras():
    
    assert demasiadas_palabras(["HOLA", "TEST", "TODO", "SOLO", "COMO"], 4) == True
    assert demasiadas_palabras(["HOLA", "TEST"], 4) == False


def horizontal(palabra, x, y, al_reves):
    if (al_reves): palabra[::-1]
    posiciones = []
    for i in range(len(palabra)):
        posiciones.append((x+i, y, palabra[i]))
    print(posiciones)


horizontal("HOLA", 0, 0, False)
""""""