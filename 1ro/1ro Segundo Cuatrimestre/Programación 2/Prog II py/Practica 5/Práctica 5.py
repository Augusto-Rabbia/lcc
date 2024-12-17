import random #para ej 1
def FrenchDeck1():
    Card_List = []
    Type = 0
    while Type <= 3:
        for i in range(1, 14):
            if Type == 0:
                Card_List.append((i, "H"))
            elif Type == 1:
                Card_List.append((i, "S"))
            elif Type == 2:
                Card_List.append((i, "D"))
            else:
                Card_List.append((i, "C"))
        Type += 1

    return(Card_List)
print(FrenchDeck1())

def Poker(D):
    Non_elected_cards = D
    Random_Card = random.randint(0, 52)
    Chosen_Card_Number = D[Random_Card][0]
    print(Chosen_Card_Number) #testing
    for i in range(5):
        if D[Random_Card][0] == Chosen_Card_Number: #checking if the randomly selected cards have the same number as the first one chosen
            Non_elected_cards.remove(D[Random_Card]) #removing the card that has already been used
            Random_Card = random.randint(0, 51 - i) #Choosing new card randomly inb the new range (the original range minus the number of cards already chosen)
            print(Random_Card) #testing
            print(len(Non_elected_cards)) #testing
            print(Chosen_Card_Number) #testing
        else: return False
    return True

print(Poker(FrenchDeck1()))

#Ej 2

def AddTime(T1, T2):
    New_Time = [T1[0] + T2[0], T1[1]+T2[1], T1[2] + T2[2]]
    if New_Time[2] >= 60:
        New_Time[2] -= 60
        New_Time[1] += 1
    if New_Time[1] >= 60:
        New_Time[1] -= 60
        New_Time[0] += 1
    if New_Time[0] >= 12:
        New_Time[0] -= 12
    return (New_Time[0], New_Time[1], New_Time[2])


print(AddTime((12, 30, 45), (12, 50, 30)))

#Ej 3
