# Lectura de la base de datos
anorexia <- read.table(file="anorexia.data", header= TRUE, 
                       sep="\t", col.names = c("Signo", "Sexo", "Edad", "Num de visitas"),
                       colClasses = c("factor","factor","numeric","numeric"))

# Acceder a las columnas de las bases de datos
anorexia$Sexo
# Acceder a las columnas de las bases de datos sin usar el operador $
attach(anorexia) 
# attach(anorexia) agrega la base de datos anorexia al camino de búsqueda,
# es decir, a la cadena de entornos donde se buscan las variables cuando
# son llamadas. Con anorexia$X estamos buscando X en el entorno de anorexia
# detach(anorexia) elimina la base de datos anorexia del camino de búsqueda
Sexo

# Ver el tipo de las columnas
class(Sexo)
class(Edad)
# Tipos de datos básicos en R:
# 1- logical (TRUE, FALSE)
# 2- numeric (2.5, 1 -puede ser decimal o no)
# 3- integral (1L -son enteros)
# 4- complex (3+2i - números complejos)
# 5- character ('a', "hola" - cadenas de caracteres o caracteres)
# 6- raw (68 6f 6c 61 -que sería hola- especifica la variable en bits crudos)
# Factor es un objeto de datos pero también es una respuesta posible a class()


# Obtener medidas resumen de tablas o columnas. Dependen del tipo de variable
summary(anorexia)
summary(Sexo)
summary(Edad)


# Crear tabla Figura 1
frec_num_visitas <- table(Num.de.visitas)
frec_rel_num_visitas <- round(frec_num_visitas/sum(frec_num_visitas), 4)
frec_acum_num_visitas <- cumsum(frec_num_visitas)
frec_acum_rel_num_visitas <- round(frec_acum_num_visitas/sum(frec_num_visitas), 4)             
tabla_num_visitas <- cbind(frec_num_visitas, frec_rel_num_visitas, frec_acum_num_visitas, frec_acum_rel_num_visitas)    


# Crear los intervalos para Edad
breaks_edad <- seq(11,35,3)
intervalos_edad <- cut(Edad, breaks = breaks_edad, right = FALSE)
# Crear tabla de Figura 2
frec_abs_edad <- table(intervalos_edad) 
frec_rel_edad <- round(frec_abs_edad / sum(frec_abs_edad),4)
frec_abs_ac_edad <- cumsum(frec_abs_edad)
# Otra manera de calcular frecuencia relativa acumulada (ver la que hicimos para Num.de.vistas)
frec_rel_ac_edad <- cumsum(frec_rel_edad) 
tabla_edad <- cbind(frec_abs_edad, frec_rel_edad, frec_abs_ac_edad, frec_rel_ac_edad)

# Cambiar la representación las "categorías" de los factores
levels(Signo) <- c("Dieta severa", "Hiperactividad", "Uso de laxantes", "Uso de ropa holgada")

# Cruzar variables categóricas
tabla_bivariada <- table(Signo, Sexo)
suma_filas <- apply(tabla_bivariada, 1, sum)
tabla_bivariada_con_totales_1 <- cbind(tabla_bivariada, suma_filas)
suma_columnas <- apply(tabla_bivariada_con_totales_1, 2, sum)
tabla_bivariada_con_totales <- rbind(tabla_bivariada_con_totales_1, suma_columnas)

