# Manejo de vectores

# 1. Crear un vector
print("Crear un vector:")

# 1.a) c function
v1 <- c(2,5,6)
print(v1)
named1 <- c("hola"=4, "tres"=2) 

# 1.b) : operator
v2 <- 1:7
print(v2)

# 1.c) seq function
v3 <- seq(1, 3, by=0.5)
print(v3)

print("")
# 2. Acceder a los elementos de un vector
print("Acceder a los elementos de un vector:")

# 2.a) Obtener un elemento usando indice positivo (empezando desde 1)
x <- v1[3]
print(x)

# 2.b) Obtener todos los elementos menos uno
v4 <- v1[-2]
print(v4)

# 2.c) Obtener varios elementos indicando sus indices en un vector
v5 <- v2[c(2,4)]
print(v5)

# 2.d) Usar un vector logico para obtener los elementos deseados
v6 <- v1[c(TRUE, FALSE, TRUE)]
print(v6)

# 2.e) A traves de su nombre
y <- named1["hola"]
print(y)

print("")
# 3. Modificar un vector
print("Modificar un vector:")

# 3.a) Usar la asignacion
v1[2] <- 4
print(v1)

# 3.b) Usar la asignacion cuando se cumpla una condicion
v2[length(v2)>4] <- 5
print(v2)

# 3.c) Reasignar un vector (si uso el mismo, puedo truncarlo por ejemplo)
v1 <- c(2,5,4,7)
v2 <- v2[1:4]
print(v1)
print(v2)

print("")
# 4. Eliminar un vector
print("Eliminar un vector")
v3 <- NULL
print("Ver que sucede si intento hacer print(v3)")

print("")
# 5. Combinar vectores
print("Combinar vectores:")

# 5.a) Concatenar vectores
v12 <- c(v1,v2)
print(v12)

# 5.b) Operar con vectores (componente a componente) (si no tienen la misma cantidad de elementos se repite el de menor longitud)
v1masv2 <- v1+v2
print(v1masv2)

# 5.c) Usar la funcion append()
v_app <- append(v1, v2, after=2)
print(v_app)

# 5.d) Usar las funciones cbind y rbind para crear matrices
print(cbind(1:4, v1))
print(rbind(1:4, v1))
