# Guardando ruta del archivo con los datos.
archivo_datos <- "C:/Users/augus/OneDrive/Desktop/Primer Cuatrimestre 3ro/PyE/Practica PyE/Laboratorio/anorexia.data"
# Leyendo los archivos
dataframe_anorexia <- read.table(archivo_datos, header = TRUE, sep = "\t")
# Averiguando la clase de cada variable
attach(dataframe_anorexia)
class(Edad)
class(Sexo)
class(Signo)
Class(Núm.de.visitas)
# Viendo la cantidad de hombres y mujeres
tabla_sexo <- table(dataframe_anorexia$Sexo)
total_hombres <- sum(tabla_sexo["M"])
total_mujeres <- sum(tabla_sexo["F"])
# Calculando las edades máxima, mínima, y promedio
edad_maxima <- max(dataframe_anorexia$Edad)
edad_minima <- min(dataframe_anorexia$Edad)
edad_promedio <- mean(dataframe_anorexia$Edad)
# Sino, con summary() podrían obtenerse todos estos datos a la vez.

# 3)
# Construyendo la tabla de frecuencias para la variable “Núm.de.visitas”
    # Creando tabla de NumVisitas
tabla_NumVisitas <- table(dataframe_anorexia$Núm.de.visitas)
  # Obteniendo los posibles valores de NumVisitas
valores_unicos_NumVisitas <- sort(unique(dataframe_anorexia$Núm.de.visitas))
    # Calculando frecuencias
frec_abs_NumVisitas <- as.numeric(tabla_NumVisitas)
frec_rel_NumVisitas <- prop.table(tabla_NumVisitas)
    # Calculando frecuencias acumuladas
frec_abs_NumVisitas_acum <- cumsum(frec_abs_NumVisitas)
frec_rel_NumVisitas_acum <- cumsum(frec_rel_NumVisitas)
    # Crear tabla con las frecuencias bindeando los datos en columnas
tabla_frec_NumVisitas_sinTotal <- cbind(valores_unicos_NumVisitas,
                               frec_abs_NumVisitas,
                               frec_rel_NumVisitas,
                               frec_abs_NumVisitas_acum,
                               frec_rel_NumVisitas_acum)
    # Añadiendo el total debajo de la tabla
total_abs_NumVisitas <- sum(frec_abs_NumVisitas)
total_rel_NumVisitas <- sum(frec_rel_NumVisitas)
tabla_frec_NumVisitas <- rbind(tabla_frec_NumVisitas_sinTotal, c("Total", total_abs_NumVisitas, total_rel_NumVisitas,"",""))
# Colocando el nombre a las columnas de la tabla
names(tabla_frec_NumVisitas) <- c("Número de Visitas",
                                  "Frecuencia absoluta",
                                  "Frecuencia relativa",
                                  "Frecuencia absoluta acumulada",
                                  "Frecuencia relativa acumulada")

# Creando la tabla para Edad
edades_intervalos_tabla <- cut(c(11,35), breaks = 8, include.lowest = TRUE)
tabla_Edad <- table(dataframe_anorexia$Edad)
frec_abs_Edad <- as.numeric(tabla_Edad)
frec_rel_Edad <- prop.table(tabla_Edad)
frec_abs_Edad_acum <- cumsum(frec_abs_Edad)
frec_rel_Edad_acum <- cumsum(frec_rel_Edad)
tabla_frec_Edad_sinTotal <- cbind(edades_intervalos_tabla,
                                        frec_abs_Edad,
                                        frec_rel_Edad,
                                        frec_abs_Edad_acum,
                                        frec_rel_Edad_acum)
total_abs_Edad <- sum(frec_abs_Edad)
total_rel_Edad <- sum(frec_rel_Edad)
tabla_frec_Edad <- rbind(tabla_frec_Edad_sinTotal, c("Total", total_abs_Edad, total_rel_Edad,"",""))
names(tabla_frec_Edad_sinTotal) <- c("Edad",
                                     "Frecuencia absoluta",
                                     "Frecuencia relativa",
                                     "Frecuencia absoluta acumulada",
                                     "Frecuencia relativa acumulada")

# 4)
# Creando tabla de Signo por Sexo: (análisis bivariado)
tabla_Signo_Sexo <- table(dataframe_anorexia$Signo, dataframe_anorexia$Sexo)
  # Hallando los totales para cada Signo
frec_abs_Signo <- as.numeric(table(dataframe_anorexia$Signo))
frec_abs_Sexo <- as.numeric(table(dataframe_anorexia$Sexo))
tabla_Signo_Sexo_Totales <- cbind(tabla_Signo_Sexo, frec_abs_Signo)
colnames(tabla_Signo_Sexo_Totales) <- c("Mujeres", "Hombres", "Total")
tabla_Signo_Sexo_Totales <- rbind(tabla_Signo_Sexo_Totales, c(frec_abs_Sexo, sum(frec_abs_Signo)))
rownames(tabla_Signo_Sexo_Totales) <- c("Dieta severa", "Hiperactividad", "Uso de laxantes", "Uso de ropa holgada", "Total")



# 5)
# NOTA, REUSAMOS CÓDIGO DEL EJERCICIO 4 (!!)
# Creando un gráfico de torta de los signos
tabla_Signo <- table(dataframe_anorexia$Signo)
frec_abs_Signo <- as.numeric(tabla_Signo)
# Calcular porcentajes de cada categoría
porcentajes_Signo <- round(frec_abs_Signo/sum(frec_abs_Signo)*100, 2)
# Nombrando los diferentes signos. Por defecto, en la función pie(), los labes es names(x), con x el vector en el primer argumento
names(frec_abs_Signo) <- c("Dieta severa", "Hiperactividad", "Uso de laxantes", "Uso de ropa holgada")
# c)
# ADICION POSTERIOR: Añadiendo los porcentajes, para esto usamos PASTE, que concatena vectores dsp de convertir a strings.
etiquetas_Signo <- paste(names(frec_abs_Signo), "\n", porcentajes_Signo, "%", sep="")
torta_Signo = pie(frec_abs_Signo, labels = etiquetas_Signo, clockwise = TRUE, 
                  main = "PRINCIPAL SIGNO VISIBLE EN PACIENTES CON ANOREXIA\nARGENTINA, OCTUBRE 2012")
# d)
mtext("Fuente: Asociación de Lucha contra la Bulimia y la Anorexia", side = 1, at = -0.5)

# 5)
# NOTA, REUSAMOS CÓDIGO DEL EJERCICIO 4 (!!)
# Creando un gráfico de barra de los signos
tabla_Signo <- table(dataframe_anorexia$Signo)
frec_abs_Signo <- as.numeric(tabla_Signo)
# Calcular porcentajes de cada categoría
# Nombrando los diferentes signos. Por defecto, en la función pie(), los labes es names(x), con x el vector en el primer argumento
names(frec_abs_Signo) <- c("Dieta severa", "Hiperactividad", "Uso de laxantes", "Uso de ropa holgada")
# c)
# ADICION POSTERIOR: Añadiendo los porcentajes, para esto usamos PASTE, que concatena vectores dsp de convertir a strings.
etiquetas_Signo <- paste(names(frec_abs_Signo), "\n", porcentajes_Signo, "%", sep="")
barplot(frec_abs_Signo,
        labels = NULL,
        horiz = TRUE,
        main = "PRINCIPAL SIGNO VISIBLE EN PACIENTES CON ANOREXIA\nARGENTINA, OCTUBRE 2012",
        ylab = "Signo", xlab = "Número de pacientes")
# d)
text(y = seq_along(frec_abs_Signo), x = -max(frec_abs_Signo)/10, labels = names(frec_abs_Signo), pos = 4)

mtext("Fuente: Asociación de Lucha contra la Bulimia y la Anorexia", side = 1, at = -0.5)

# 6) a-b-c
orden_Signo <- rev(order(frec_abs_Signo))
frec_abs_Signo_ordenado <- frec_abs_Signo[orden_Signo]
nombres_Signo_ordenados <- names(frec_abs_Signo)[orden_Signo]

barplot(frec_abs_Signo_ordenado,
        horiz = TRUE, 
        
        main = "PRINCIPAL SIGNO VISIBLE EN PACIENTES CON ANOREXIA\nARGENTINA, OCTUBRE 2012",
        xlab = "Frecuencia", ylab = "Signo")

# d - Como pija pongo los labels horizontales, y qué le pasa al texto XD
mtext("Fuente: Asociación de Lucha contra la Bulimia y la Anorexia", side = 1, at = -0.5)





