def factorial(n):
    x = 1
    for i in range(1,n+1):
        x = x*i
    print(x)

factorial(6)

def FtoC(n):
    return (n-32)*5/9

print(FtoC(32))

def tabla_FtoC():
    tabla = ""
    for i in range(0,120,10):
        tabla += str(FtoC(i)) + "  |  "
    return(tabla)
print(tabla_FtoC())

def interes(C, T, I):
    return C * (1+I/100)**T
print(interes(100,1,10))

def fibonacci(n):
    F = 0
    for i in range(n+1):
        F += i
    return(F)
print(fibonacci(5))

def fibonacci2(n):
    return int(n*(n+1)/2)
print(fibonacci2(5))
