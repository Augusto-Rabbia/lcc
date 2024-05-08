#Ej 1
def RepetitionSum(L, E):
    RepsCount = 0
    for i in L:
        if E == i: RepsCount += 1
    return RepsCount

def test_RepetitionSum():
    assert RepetitionSum([1,1,1,1,2,3,4,5,6,6,1,10], 1) == 5
test_RepetitionSum()

def LocateElement_first(L, E):
    Pos = 0 #current element being checked (position of the element)
    if E in L:
        for i in L:
            if i == E:
                return Pos
            else: Pos += 1
    else: return "The element is not in the list"

def test_LocateElement_first():
    assert LocateElement_first([1,1,1,1,2,3,4,5,6,6,1,10], 1) == 0
    assert LocateElement_first([1,1,1,1,2,3,4,5,6,6,1,10], 2) == 4
test_LocateElement_first()

def LocateElement_All(L, E):
    Poss = []
    Location = 0
    List = L
    while E in List:
        Location += LocateElement_first(List, E) + 1
        Poss.append(Location-1)
        List = L[Location:]
    return Poss

def test_LocateElement_All():
    assert LocateElement_All([1,1,1,1,2,3,4,5,6,6,1,10,1], 1) == [0,1,2,3,10,12]
    assert LocateElement_All([1,1,1,1,2,3,4,5,6,6,1,10], 2) == [4]
test_LocateElement_All()











