print("Ej 1-3\n")

#pares: number, number -> number
#recibe dos numeros y obtiene los numeros pares entre estos
def pares(desde, hasta):
    for n in range(desde, hasta):
        if n % 2 == 0:
            print(n)

pares(10, 20)

print("\n Ej 4-6 \n")
#pares: number, number -> number
#recibe dos numeros y suma todos los numeros entre estos
def sumar_rango(desde, hasta):
    total=0
    for n in range(desde, hasta):
        total += n
    print(total)

sumar_rango(0, 5)

print("\n Ej 7-8 \n")

def print_mult(str, n):
    print(str * n)
print_mult("hola", 3)

print("\n Ej 9 \n")

def sumar(n1, n2):
    print(n1 + n2)
def restar(n1, n2):
    print(n1 - n2)
def multiplicar(n1, n2):
    print(n1 * n2)
def dividir(n1, n2):
    print(n1 / n2)

while True:
    operacion = int(input("Qué operación desea realizar? \n1.Suma \n2.Resta \n3.Mult \n4.Div \n5.Salir\n"))
    if operacion != 5:
        N1 = int(input("Primer Número"))
        N2 = int(input("Segundo Número"))
        if operacion == 1:
            sumar(N1, N2)
        if operacion == 2:
            restar(N1, N2)
        if operacion == 3:
            multiplicar(N1, N2)
        if operacion == 4:
            dividir(N1, N2)
        print("\n----- nueva operacion")
    else:
        print("Cerrando programa...")
        break

