import random


""" personas_ciudad toma un archivo de personas, y 
devuelve una tupla con 2 diccionarios, uno con menores y otro con mayores,
de las ciudades, y a cada una le corresponde una lista con todas las personas en esta ciudad

personas_ciudad : Str -> Tuple(Dict[Str: List[Str]])

Ejemplos:
Primer jugador mayor rosarino == "JOSE DELGADO"
Primer jugador menor de Villa constitucion == "RECAREDO MURILLO"
Primera jugadora mayor santafesina == "CARMEN CORRALES"
"""
def personas_ciudad(txt):
    particip_mayores = []
    particip_menores = []
    with open(txt, "r") as f:
        for i in f.readlines():
            datos = i.split(",")
            if int(datos[1]) < 18:
                particip_menores.append((datos[0], datos[2].strip()))
            else:
                particip_mayores.append((datos[0], datos[2].strip()))
                
    mayores_por_ciudad = dict()
    menores_por_ciudad = dict()
    for i in particip_mayores:
        if i[1] not in mayores_por_ciudad:
            mayores_por_ciudad[i[1]] = [i[0]]
        else:
            mayores_por_ciudad[i[1]].append(i[0])
    for i in particip_menores:
        if i[1] not in menores_por_ciudad:
            menores_por_ciudad[i[1]] = [i[0]]
        else:
            menores_por_ciudad[i[1]].append(i[0])
    return (mayores_por_ciudad, menores_por_ciudad)
    

def test_personas_ciudad():
    assert personas_ciudad("jugadores.txt")[0]["Rosario"][0] == "JOSE DELGADO"
    assert personas_ciudad("jugadores.txt")[1]["Villa Constitucion"][0] == "RECAREDO MURILLO"
    assert personas_ciudad("jugadores.txt")[0]["Santa Fe"][0] == "CARMEN CORRALES"


""" 
peleas_por_ciudad recibe la lista con todas las personas
de una ciudad. Luego, selecciona a todos los elementos de
a dos de manera aleatoria, los elimina de la lista original
y agrega a uno de los dos una nueva lista, la cual será devuelta a la
función principal. 
Además, añade al registro quién gano y quién fue
eliminado.

peleas_por_ciudad : List -> List
"""
def peleas_por_ciudad(ciudad):
    asignados = ciudad  # Jugadores que aún no combatieron
    ganadores = [] # Jugadores que pasan a la siguiente ronda
    if len(asignados) != 1:
        while len(asignados) > 1:
            persona1 = asignados[random.randint(0, len(asignados)-1)]
            persona2 = asignados[random.randint(0, len(asignados)-1)]
            if persona1 != persona2:
                ganadores.append(persona1)
                asignados.remove(persona1) # No vuelven a pelear en la ronda
                asignados.remove(persona2)
        
                with open("registro_de_juego.txt", "a") as f:
                    f.write(persona1 + " elimino a " + persona2 + "\n")

        return ganadores


''' dist_ciudades toma un archivo de texto y crea a partir de él 
un diccionario de ciudades, cuyos valores son diccionarios que contienen 
una ciudad, y la distancia entre la primer y la segunda ciudad

dis_ciudades : Str -> Dict[Dict[Str, Int]]

Ejemplos: 
Distancia entre CABA y: Cordoba: 696.4, Rio Cuarto: 617.8,  'Rosario': 299.9,
Distancia entre Villa Maria y: CABA: 552.7, Cordoba: 153.3, Serodino: 255.1
Distancia entre VGG y: Rosario: 13.9, Santa Fe: 182.5, Villa constitucion: 53.7
'''
def dist_ciudades(txt):
    ciudades = dict()
    with open(txt, "r") as f:
        for line in f.readlines():
            datos = line.split(",")
            city = datos[0].strip() # Primer ciudad
            city2 = datos[1].strip() # Segunda ciudad
            dist = float(datos[2].strip()) # Distancia entre ambas
            if city not in ciudades:
                ciudades[city] = dict()
                ciudades[city][city2] = dist
            else:
                ciudades[city][city2] = dist
            # También queremos relacionar a la segunda ciudad con la primera
            if city2 not in ciudades:
                ciudades[city2] = dict()
                ciudades[city2][city] = dist
            else:
                ciudades[city2][city] = dist
    return ciudades

def test_dist_ciudades():
    assert dist_ciudades("distancias.txt")["CABA"] == {'Cordoba': 696.4, 'Rio Cuarto': 617.8, 
                                                            'Rosario': 299.9, 'Santa Fe': 468.5,
                                                            'Serodino': 349.8, 'Villa Constitucion': 257.0,
                                                            'Villa Gobernador Galvez': 293.1, 'Villa Maria': 552.7}
    
    assert dist_ciudades("distancias.txt")["Villa Maria"] == {'CABA': 552.7, 'Cordoba': 153.3, 'Rio Cuarto': 133.7, 
                                                                   'Rosario': 257.5, 'Santa Fe': 302.7, 'Serodino': 255.1, 
                                                                   'Villa Constitucion': 313.4, 'Villa Gobernador Galvez': 268.2}
                                                        
    assert dist_ciudades("distancias.txt")["Villa Gobernador Galvez"] == {'CABA': 293.1, 'Cordoba': 410.3, 'Rio Cuarto': 399.5, 
                                                                                'Rosario': 13.9, 'Santa Fe': 182.5, 'Serodino': 64.6, 
                                                                                'Villa Constitucion': 53.7, 'Villa Maria': 268.2}

"""
ciudad_valida_mas_cercana calcula cuál es la ciudad más
cercana que cumpla con los requisitos del juego para que
haya un enfrentamiento entre las personas restantes en estas.
Se considera como válida a una ciudad cuando está a una
distancia menor al valor N, que indica la distancia máxima
entre ciudades para que combatan, y todavía tiene jugadores.

ciudad_valida_mas_cercana: Dict[Str, List[Str]], Str, Number -> Str

Ejemplos:
CABA con 1 jugador, Villa Constitucion con ninguno, VGG con 1, el resto con 1 >> VGG
Villa Maria con 1 jugador, el resto sin jugadores >> "No existen posibles contrincantes"
VGG con 1 jugador, Rosario con 1 jugador, el resto con 1 >> Rosario
Santa Fe con 1 jugador, Rosario con 1, con 300 de distancia >> "No existen posibles contrincantes"
"""
def ciudad_valida_mas_cercana(ciudades_restantes, pueblo, n):
    distancias = dist_ciudades("distancias.txt")
    ciudades_cercania = sorted(list(distancias[pueblo].values())) # Las distancias entre la ciudad elegida (pueblo) y el resto, en orden ascendente
    orden = 0
    while orden < len(ciudades_cercania):
        for i in distancias[pueblo]:
        # Por cada distancia, queremos ver si:
        #      La distancia entre una ciudad y la elegida             La distancia entre estas       La ciudad aún tiene
        #      coincide con la dist n° orden                          ciudades es menor a n         participantes adentro
            if distancias[pueblo][i] == ciudades_cercania[orden] and distancias[pueblo][i] <= n and i in ciudades_restantes:
                return i
        # Si ninguna junta todos los requisitos, comprobamos con la segunda distancia más corta
        orden += 1
    return "No existen posibles contrincantes"

def test_ciudad_valida_mas_cercana():
    assert ciudad_valida_mas_cercana({"CABA": ["Noelia"], "Villa Gobernador Galvez": ["Ramiro"]}, "CABA", 300) == "Villa Gobernador Galvez"
    assert ciudad_valida_mas_cercana({"Villa Maria": ["Camilo"]}, "Villa Maria", 1000) == "No existen posibles contrincantes"
    assert ciudad_valida_mas_cercana({"Villa Gobernador Galvez": ["Paloma"],"Villa Constitucion": ["Jorge"], "Rosario": ["Pedro"]},"Villa Gobernador Galvez", 500) == "Rosario"
    assert ciudad_valida_mas_cercana({"Santa Fe": ["Marina"], "Rosario": ["Melisa"]}, "Santa Fe", 300) == "No existen posibles contrincantes"


""" batalla_entre_ciudades toma una ciudad, y la más cercana
que aún cuente con jugadores dentro de ella, que esté a una 
distancia menor a la especificada por el jugador. 
Luego, aleatoriamente, decide quién gana y elimina a la 
ciudad perdedora de los participantes.
En caso de que no haya ninguna ciudad con jugadores, o ninguna a la distancia
necesaria, determina al jugador de la ciudad como ganador regional.
Registra la batalla en "registro_de_juego.txt".

batalla_entre_ciudades: Dict[Str, List[Str]], Str, Number
""" 
def batalla_entre_ciudades(jugadores, ciudad, n):
    jug1 = jugadores[ciudad][0]
    ciudad_mas_cercana = ciudad_valida_mas_cercana(jugadores, ciudad, n)
    with open("registro_de_juego.txt", "a") as f:
        if ciudad_mas_cercana == "No existen posibles contrincantes":
            f.write(jug1 + " es ganador en su region.\n")
            del jugadores[ciudad]
        else:
            jug2 = jugadores[ciudad_mas_cercana][0]
            if random.randint(0,1) == 1:
                del jugadores[ciudad]
                f.write(jug2 + " elimino a " + jug1 + "\n")
            else:
                del jugadores[ciudad_mas_cercana]
                f.write(jug1 + " elimino a " + jug2 + "\n")


"""partipantes_restantes toma un conjunto de jugadores y 
determina cuántas personas quedan en juego

participantes_restantes: Dict[Str, List[str]] -> Number

Ejemplos:
3 restantes en CABA, 3 en rosario == 6
1 en Cordoba == 1
2 en Serodino, 1 en Villa Maria == 3
"""
def participantes_restantes(jugadores):
    restantes = 0
    for i in jugadores.values():
        restantes += len(i)
    return restantes

def test_participantes_restantes():
    assert participantes_restantes({"Caba": ["Juan", "Marcos", "Pablo"], "Rosario": ["Paola", "Carla", "Matias"]}) == 6
    assert participantes_restantes({"Cordoba": ["Roberto"], "Rosario": []}) == 1
    assert participantes_restantes({"Serodino": ["Marina", "Vicente"], "Villa Maria": ["Joaquin"]}) == 3

"""game_jugadores se encarga de coordinar de manera global 
todas las funciones anteriormente definidas para que  el juego 
se desarrolle de la manera esperada.

game_jugadores: Number, Dict[Str, List[Str]]
"""
def game_jugadores(N, jugadores):    
    while True:
        jugadores_inicio_ronda = participantes_restantes(jugadores)
        num_ciudades_con_mas_de_1_persona = len(jugadores)
        for city in jugadores:
            auxiliar_jugadores = jugadores.copy() # Para poder modificar la cantidad de jugadores en medio de la iteración
            if len(jugadores[city]) == 1:
                # Para determinar si se deben llevar a cabo las batallas entre ciudades
                num_ciudades_con_mas_de_1_persona -= 1

            # Caso en que queda 1 persona por ciudad
            if num_ciudades_con_mas_de_1_persona == 0:
                for ciudad in jugadores:
                    # En caso de que haya sido atacada por otra ciudad y haya perdido
                    if ciudad in auxiliar_jugadores:
                        batalla_entre_ciudades(auxiliar_jugadores, ciudad, N)

                jugadores = auxiliar_jugadores # Una vez hechas las peleas entre ciudad, para continuar con la iteración for

            else:
                # Caso en que hayan ciudades con más de un jugador
                if len(jugadores[city]) > 1:
                    jugadores[city] = peleas_por_ciudad(jugadores[city])                    
        jugadores_final_ronda = participantes_restantes(jugadores) 

        if jugadores_inicio_ronda == jugadores_final_ronda:
            # Si no se eliminó a ningun jugador, significa que nadie pudo asesinar a nadie, por lo tanto, los restantes son ganadores en su region
            break
        
"""game permite al jugador establecer una distancia máxima para los
asesinatos entre ciudades, y a partir de ese número, llama a la función
game_jugadores, tanto para menores como para mayores, dejando de a qué categoría 
pertenecen los registros (mayores o menores). Al terminar de ejecutar el juego,
imprime un mensaje señalando la finalización del programa.

game: Number
"""
def game(N):
    with open("registro_de_juego.txt", "a") as f:
        f.write("Mayores de edad\n\n")
    game_jugadores(N, personas_ciudad("jugadores.txt")[0]) # Jugadores mayores
    with open("registro_de_juego.txt", "a") as f:
        f.write("\n=========================================\nMenores de edad\n\n")
    game_jugadores(N, personas_ciudad("jugadores.txt")[1]) # Jugadores menores
    print("Los resultados fueron registrados en 'registro_de_juego.txt'")
    

game(int(input("Distancia máxima para los enfrentamientos entre jugadores de diferentes ciudades: ")))