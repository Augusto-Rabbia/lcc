#ej 1

def positionsMultiple(L,N):
    NewList = []
    for i in range(int(len(L)/N)+1):
        NewList.append(L[i*N])
    return NewList

print(positionsMultiple([1,2,3,4,5,6,7],2))
print(positionsMultiple([1,2,3,4,5,6,7],3))

#ej 2

def accumulatingSum(L):
    Sum=0
    SumList = []
    for i in L:
        Sum += i
        SumList.append(Sum)
    return SumList

print(accumulatingSum([1,2,3,4]))

#ej 3

def delFirstLast(L):
    NoFirstLastList = []
    for i in L[1:-1]:
        NoFirstLastList.append(i)
    return NoFirstLastList

print(delFirstLast([1,2,3,4,5,6,7,8,9]))

#ej 4

def is_in_order(L):
    for i in range(len(L)-1):
        if L[i] > L[i+1]:
            return False
    return True

print(is_in_order([1,2,3,4,5,6]))
print(is_in_order([1,2,3,4,5,2]))
print(is_in_order(["a","b","c"]))

#ej 6

def no_dups(L):
    NodupsList = []
    for i in L:
        if i not in NodupsList:
            NodupsList.append(i)
    return NodupsList

print(no_dups([1,2,1,1,1,3,2,4,5,6,2,10,9]))

#ej 7

def unique_elements(L):
    return len(no_dups(L))

print(unique_elements([1,2,3,1,1,1,1,4,5,3,4,4,3,2,1,6]))

#ej 8

def dicotomy_search(L,Word):
    if L[int(len(L)/2)] > Word:
        if Word in L[:int(len(L)/2)]:
            return True
        else:
            return False
    elif Word in L[int(len(L)/2):]:
            return True
    else:
            return False

print(dicotomy_search(["abc", "bread", "feel", "hello","hi","how", "normal", "yay"],"feel"))
