import random
cols = ['MARMOTEAR', '0AUGUSTO0', 'EMPLOMADO', 'ASANDWICH', 'LCINCEL00', 'AZEINAPaN', 'SEMPaNADA', 'aPANCHObc', 'TROQUILO0']
palabras = ['PAN']

#palabras = ['MARMOTEAR', 'EMPLOMADO', 'TROQUILO', 'SANDWICH', 'EMPANADA', 'AUGUSTO', 'CINCEL', 'PANCHO', 'ZEINA', 'ALAS', 'PAN']

abcdario = "abcdefghijklmnopqrstuvwxyz"


col_num = 0

for palabra in palabras:
    for col in cols:
        if palabra in col.upper() and palabra not in col:
            for letra in col:
                if letra.islower():
                    terminado = False
                    cols[col_num] = cols[col_num].replace(letra, abcdario.replace(letra, "")[random.randint(0, len(abcdario)-2)])
                    #
        col_num += 1



print(cols)
                    
def verificarRepeticiones(sopa, lista_palabras, direccion, deltaX, deltaY):
    terminado = True
    y = 0
    x = 0
    for palabra in palabras:
        for i in direccion:
            if palabra in i.upper() and palabra not in col:
                for letra in i:
                    if letra.islower():
                        terminado = False
                        sopa[y][x] = direccion[y].replace(letra, abcdario.replace(letra, "")[random.randint(0, len(abcdario)-2)])
                x += deltaX
            y += deltaY

    return terminado

# while not terminado:
#     terminado = verificarRepeticiones()
