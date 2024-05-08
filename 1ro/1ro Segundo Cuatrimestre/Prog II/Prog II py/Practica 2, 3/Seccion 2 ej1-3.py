import math

def num_1toX(n):
    for i in range(n):
        print(i)

def dominosCustom(n):
    for i in range(11*n+1):
        if i % 10 <= n:
            print(math.floor(i / 10), i % 10)

dominosCustom(9)
