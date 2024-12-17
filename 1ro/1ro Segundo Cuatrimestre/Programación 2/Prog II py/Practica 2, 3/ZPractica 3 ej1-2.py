import random
def diceroll():
    return(random.randrange(1,7))
def ManyDiceRolls(A):
    A = int(A)
    Results = ""
    while A > 0:
        Results += str(diceroll()) + " "
        A -= 1
    return Results

print(ManyDiceRolls(input("How many die do you want to roll?: ")))