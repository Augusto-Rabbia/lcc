#Ej 1
def Tuples_to_dict(L):
    dict = {}
    for i in L:
        if i[0] in dict:
            dict[i[0]] += [i[1]]
        else: dict [i[0]] = [i[1]]
    return dict
print(Tuples_to_dict([("hi", 1), (1, 2), (5, 7), (1, 7)]))

#Ej 2
def Word_Count(Str):
    dict = {}
    for i in Str.split():
        if i in dict:
            dict[i] += 1
        else: dict[i] = 1
    return dict
print(Word_Count("Qué lindo dia que hace hoy que hoy que" ))




while True:
    print(money)







