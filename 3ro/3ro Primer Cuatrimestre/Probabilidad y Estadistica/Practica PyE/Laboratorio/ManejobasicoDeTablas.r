#setwd("E:/")

# Manejo básico de tablas
print("Manejo basico de tablas")

print("")
# 1. Crear una tabla mediante la funcion as.table
print("Crear una tala con as.table()")
tab1 <- as.table(rbind(c(143,100,23), c(120,123,45)))
dimnames(tab1) <- list(genero = c("M", "F"), equipo = c("RC", "NOB", "Ninguno"))
print(tab1)

# 2. Guardar una tabla creada en un archivo - investigar la funcion save
print("Guardar una tabla creada en un archivo")
write.csv(tab1, file="SimpatizantesEquiposRosarioPorGenero.csv")

# 3. Cargar a nuestro programa datasets ya disponibles en el paquete datasets (no requiere instalacion)
print("Cargar a nuestro programa datasets ya disponibles en el paquete datasets")
data(iris)

# 4. Cargar una tabla desde un archivo
print(" Cargar una tabla desde un archivo")
anorexia <- read.table("anorexia.data", sep='\t')

# 5. Cargar una tabla de un archivo excel
print("Cargar una tabla de un archivo excel")
# Instalamos el paquete
install.packages("readxl")
# Cargamos el paquete
library(readxl) 
# Importamos
basepartos <- read_xls("Base_partos.xls")

# 6. Obtener las primeras n lineas de una tabla
print("Obtener las primeras n lineas de una tabla")
head(anorexia)

# 7. Resumir la informacion de un dataset
print("Resumir la informacion de un dataset")
summary(basepartos)

# 8. Combinar tablas
notas <- data.frame(Legajo = c("L-1", "G-1", "H-1"), Nota = c(70, 85, 90))
nombres <- data.frame(Legajo = c("G-1", "L-1", "H-1"), Nombre = c("Beatriz Gimenez", "Adrian Lopez", "Camila Hernandez"))
tab <- merge(notas, nombres, by='Legajo')
