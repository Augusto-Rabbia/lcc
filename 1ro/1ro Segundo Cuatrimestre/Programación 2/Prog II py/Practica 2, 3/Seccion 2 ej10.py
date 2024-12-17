def TwoInt2ndHigher():
    print("If the second integer is higher than the first one, the two integers will be returned.")
    Int1= input("Type the first integer: ")
    Int2= input("Type the second integer: ")
    while int(Int1) >= int(Int2):
        Int2= input("That one's not higher than the first one, type a new second integer: ")
    return(Int1 + "  " + Int2)

print(TwoInt2ndHigher())