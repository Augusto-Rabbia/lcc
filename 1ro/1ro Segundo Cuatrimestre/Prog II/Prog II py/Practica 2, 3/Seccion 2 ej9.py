def promediar():
    Num = 0
    Cant = 1
    while True:
        Num += float(input("Añada un número para promediar: "))
        if input("Desea añadir más números para promediar?(Y/N): ") == "Y":
            Cant +=1
            pass
        else: break
    return(Num/Cant)

print(promediar())