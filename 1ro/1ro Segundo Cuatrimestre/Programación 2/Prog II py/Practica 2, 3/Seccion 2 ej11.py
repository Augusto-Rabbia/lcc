def MultiplesUntilB(Int1, Int2):
    Quantity = 0
    for i in range(Int2):
        if i*Int1 < Int2:
            Quantity += 1
    return Quantity

print(MultiplesUntilB(5,50))

def MultiplesUntilB2(Int1, Int2):
    Quantity = 1
    Num = Int1
    while Num < Int2:
        Num = Num*Quantity
        Quantity += 1
    return Quantity

print(MultiplesUntilB(5,100))