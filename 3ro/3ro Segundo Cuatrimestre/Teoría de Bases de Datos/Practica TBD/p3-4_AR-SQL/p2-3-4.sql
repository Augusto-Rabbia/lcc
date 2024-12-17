/*  Práctica AR en SQL  */
-- a
SELECT * FROM J;
-- b
SELECT * FROM J WHERE ciudad = 'Londres';
-- c
SELECT SNum FROM SPJ WHERE JNum = 'J1';
-- d
SELECT * FROM SPJ WHERE cant >= 300 AND cant <= 750;
-- e
SELECT DISTINCT color, ciudad FROM P;
-- f
SELECT JNum, PNum, SNum FROM S, P, J
	WHERE S.ciudad = P.ciudad AND S.ciudad = J.ciudad;
-- g
SELECT JNum, PNum, SNum FROM S, P, J
	WHERE S.ciudad <> P.ciudad AND S.ciudad <> J.ciudad;
-- h
SELECT PNum FROM SPJ
	WHERE SNum IN (SELECT SNum FROM S
						WHERE ciudad = 'Londres';
-- i
SELECT PNum FROM SPJ
	WHERE	SNum IN (SELECT SNum FROM S WHERE ciudad = 'Londres')
	AND		JNum IN (SELECT JNum FROM J WHERE ciudad = 'Londres');
-- j
SELECT PNum FROM SPJ
	WHERE (SNum, JNum) IN (SELECT SNum, JNum FROM S, J
							WHERE S.ciudad = J.ciudad);
-- k
SELECT JNum FROM SPJ R1
	WHERE SNum NOT IN (SELECT SNum FROM SPJ R2
						WHERE	SNum IN (SELECT SNum FROM S WHERE ciudad = 'Londres')
						AND		PNum IN (SELECT PNum FROM P WHERE color = 'rojo'));
-- l
SELECT JNum FROM SPJ R1
	WHERE SNum = 'S1'
	AND JNum NOT IN (SELECT JNum FROM SPJ R2
						WHERE SNum <> 'S1');
--	AND NOT EXISTS (SELECT * FROM SPJ R2
--						WHERE R1.JNum = R2.JNum
--						AND SNum <> 'S1')
-- Equivalentes
-- m
SELECT PNum FROM SPJ
	WHERE JNum IN (SELECT JNum FROM J WHERE ciudad = 'Londres');
-- n
SELECT SNum FROM SPJ R1
	WHERE JNum IN (SELECT JNum FROM SPJ R2
					WHERE NOT EXISTS (SELECT * FROM SPJ R3
										WHERE R3.PNum = R2.PNum
										AND R3.PNum <> R2.PNum));
-- o
SELECT SNum FROM SPJ R1
	WHERE NOT EXISTS (SELECT * FROM SPJ R2
						WHERE R1.PNum = R2.PNum
						AND R2.SNum = 'R1');
-- p
SELECT nombre FROM S
	WHERE SNum IN (SELECT
-- Me dio paja seguir y tenía que estudiar para otro parcial también






/* ---------------------------- p3 ---------------------------- */
-- 24
SELECT DISTINCT JNum FROM SPJ
	WHERE PNum IN (SELECT PNum FROM SPJ
						WHERE SNum = 'S1');
-- 25
SELECT DISTINCT SNum FROM SPJ
	WHERE PNum IN (SELECT PNum FROM SPJ
					WHERE SNum IN (SELECT SNum FROM SPJ
									WHERE PNum IN (SELECT PNum FROM P WHERE color = 'Rojo')));
-- 26
SELECT SNum FROM S
	WHERE situacion < (SELECT situacion FROM S WHERE SNum = 'S1');
/* EXISTS */
-- 28
SELECT DISTINCT PNum FROM SPJ
	WHERE EXISTS (SELECT * FROM J 
					WHERE SPJ.JNum = J.JNum AND J.ciudad = 'Londres');
-- 29
SELECT DISTINCT JNum FROM SPJ A
	WHERE EXISTS (SELECT * FROM SPJ B
					WHERE A.PNum = B.PNum AND SNum = 'S1');
-- 30
SELECT DISTINCT JNum FROM SPJ A
	WHERE NOT EXISTS (SELECT * FROM SPJ B
						WHERE A.JNum = B.JNum
						AND SNum IN (SELECT SNum FROM S WHERE ciudad = 'Londres')
						AND PNum IN (SELECT PNum FROM P WHERE color = 'rojo'));
-- 31
SELECT DISTINCT JNum FROM SPJ A
	WHERE --SNum = 'S1'
	NOT EXISTS (SELECT * FROM SPJ B
					WHERE JNum = A.JNum
					AND SNum <> 'S1');
/* ADICIONALES */
-- 1
SELECT PNum FROM SPJ, J
	WHERE SPJ.JNum = J.JNum AND J.ciudad = 'Londres'
	GROUP BY PNum
	HAVING COUNT(DISTINCT JNum) = (SELECT COUNT(JNum) FROM J WHERE J.ciudad = 'Londres') 
-- 2
-- Muy difícil este, sólo me salió una vez en 5 intentos.

/* ---------------------------- p4 ---------------------------- */
/* I */
-- 1
SELECT nom FROM Club
	WHERE	presupuesto > 5000000
	AND		CNum IN (SELECT CNum FROM Participacion Par, Competicion Com
						WHERE	Par.PNum = Com.PNum
						AND		Com.categoria >= 2);
-- 2
SELECT nom FROM Club
	WHERE CNum IN (SELECT CNum FROM Participacion Par1
					WHERE puesto = 1
					AND NOT EXISTS (SELECT * FROM Participacion Par2
										WHERE Par1.CNum = Par2.CNum
										AND puesto <> 1));
-- 3
SELECT nom FROM Club
	WHERE NOT EXISTS (SELECT PNum FROM Competicion Com
						WHERE CNum NOT IN (SELECT CNum FROM Participacion Par
											WHERE	Par.PNum = Com.PNum));
-- 4
SELECT nom FROM Club
	WHERE CNum IN (SELECT CNum FROM Participacion WHERE PNum = 'P1')
	AND CNum IN (SELECT CNum FROM Participacion WHERE PNum = 'P2');
-- 5
SELECT nom, presupuesto FROM Club
	WHERE NOT EXISTS (SELECT * FROM Participacion WHERE CNum = Club.CNum AND puesto = 1);
/* II */
-- 1
SELECT PNum, fecha, res FROM Partidas
	WHERE (J1Num IN (SELECT JNum FROM Jugadores WHERE nacionalidad = 'Argentino')
	OR J2Num IN (SELECT JNum FROM Jugadores WHERE nacionalidad = 'Argentino'))
	AND CNum IN (SELECT CNum FROM Campeonatos WHERE categoria = 'Junior');
-- 2
SELECT nombre FROM Jugadores
	WHERE JNum IN (SELECT JNum FROM Partidas
					WHERE CNum = 'C1'
					OR CNum = 'C7');
-- 3
SELECT nombre FROM Jugadores
	WHERE JNum IN (SELECT JNum FROM Partidas
					WHERE CNum = 'C1');
	AND JNum IN (SELECT JNum FROM Partidas
					WHERE CNum = 'C7');
-- 4
SELECT cnombre FROM Campeonatos
	WHERE CNum IN (SELECT CNum FROM Partidas
					WHERE (SELECT nacionalidad FROM Jugadores WHERE JNum = J1Num) = (SELECT nacionalidad FROM Jugadores WHERE JNum = J2Num));
-- 5
SELECT nombre FROM Jugadores
	WHERE NOT EXISTS (SELECT * FROM Campeonatos
						WHERE CNum IN (SELECT CNum FROM Partidas
										WHERE	J1Num = JNum
										AND		J2Num = JNum));
/* III */
-- 1
SELECT Nombre FROM Persona
	WHERE nomMadre, nomPadre 
			IN (SELECT nomMujer, nomVaron FROM Matrimonio
					WHERE fechaNac < fechaRealizacion
					OR fechaNac > fechaFin);
-- 2
SELECT nombre FROM Persona P1
	WHERE NOT EXISTS (SELECT nombre FROM Persona P2
						WHERE P1.nomPadre = P2.nomPadre
						AND P1.nomMadre = P2.nomMadre		-- Tiene mismos padres
						AND P1.fechaNac > P2.fechaNac)		-- Nació antes
	AND nomMadre, nomPadre
		IN (SELECT nomMujer, nomVaron FROM Matrimonio -- Los padres estaban casados durante o luego de su nacimiento.
				WHERE nomMujer = nomMadre AND nomVaron = nomPadre
				AND		fechaFin < P1.fechaNac);

-- Suponemos que un hijo nacido antes de un matrimonio también se considera el primogénito del matrimonio
-- 3
SELECT nombre FROM Persona posiblePadre
	WHERE NOT EXISTS (SELECT * FROM Persona posibleHijo
						WHERE posibleHijo.nomMadre = posiblePadre.nombre
						OR posibleHijo.padre = posiblePadre.nombre);
-- 4
SELECT nombre FROM Persona
	WHERE NOT EXISTS (SELECT * FROM Matrimonio
						WHERE nomMujer = nombre
						OR nomVaron = nombre);
-- 5
SELECT nombre FROM Persona
	WHERE sexo = 'F' -- Opcional pero más eficiente
	AND EXISTS (SELECT * FROM Persona WHERE sexo = 'M' AND nomMadre = nombre)
	AND EXISTS (SELECT * FROM Persona WHERE sexo = 'F' AND nomMadre = nombre);
/* Manuel lo hizo así:
SELECT nomMadre FROM Persona 
	GROUP BY nomMadre
	HAVING COUNT (DISTINCT sexo) = 2;
Mucho más lindo
*/
-- 6
SELECT nomMujer, nomVaron FROM Matrimonio
	WHERE EXISTS (SELECT * FROM Persona WHERE sexo = 'M' AND nomMadre = nomMujer AND nomPadre = nomVaron)
	AND NOT EXISTS (SELECT * FROM Persona WHERE sexo = 'F' AND nomMadre = nomMujer AND nomPadre = nomVaron);
-- 7
SELECT nombre FROM Persona P1
	WHERE EXISTS (SELECT * FROM Persona P2
					WHERE nomMadre = P1.nombre OR nomPadre = P1.nombre -- Es hijo de P1
					AND nomMadre, nomPadre 
							IN (SELECT nomMujer, nomVaron FROM Matrimonio M1
									WHERE P2.fechaNac > fechaRealizacion AND P2.fechaNac < fechaFin -- Nació durante el matrimonio
									AND NOT EXISTS (SELECT * FROM Matrimonio M2						-- Es el primer matrimonio de P1
														WHERE M2.fechaRealizacion < M1.fechaRealizacion
														AND ((P1.sexo = 'M' AND M2.nomVaron = M1.nomVaron)
														OR 	 (P1.sexo = 'F' AND M2.nomMujer = M1.nomMujer)) ));
-- Pasar a primer matrimonio de P1:




/* Parcial 2020 */
-- c
SELECT persona FROM Frecuenta
	WHERE NOT EXISTS (SELECT bar FROM Sirve
						WHERE Sirve.bar = Frecuenta.bar
						AND NOT EXISTS (SELECT cerveza FROM Gusta
											WHERE Sirve.cerveza = Gusta.cerveza
											AND Gusta.persona = Frecuenta.Persona))



SELECT Persona FROM Frecuenta F1
	WHERE NOT EXISTS (SELECT Bar FROM Sirve
						WHERE Sirve.Bar NOT IN (SELECT Bar FROM Frecuenta F2
													WHERE F1.Persona = F2.Persona
													AND F2.Bar = Sirve.Bar))