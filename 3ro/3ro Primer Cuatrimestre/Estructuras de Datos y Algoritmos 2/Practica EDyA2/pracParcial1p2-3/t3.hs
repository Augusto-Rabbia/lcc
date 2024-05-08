-- Ej1 
type Color = (Float,Float,Float)
mezclar :: Color -> Color -> Color
mezclar (r1,g1,b1) (r2,g2,b2) = ((b1+b2)/2, (b1+b2)/2, (b1+b2)/2)

--Ej2
type Linea = ([Char], Int)
vacia :: Linea
vacia = ([], 0)
moverIzq :: Linea -> Linea
moverIzq (xs, p) | p <= 0 = (xs,p)
                 | otherwise = (xs,p-1)
moverDer :: Linea -> Linea
moverDer (xs, p) | p >= length xs = (xs,p)
                 | otherwise = (xs,p-1)
moverIni :: Linea -> Linea
moverIni (xs, p) = (xs, 0)
moverFin :: Linea -> Linea
moverFin (xs, p) = (xs, (length xs)-1)
insertar :: Char -> Linea -> Linea
insertar c (xs, 0) = (c: xs, 0)
insertar c (xs, p) = (insertAt c xs p, p+1)

insertAt :: Char -> [Char] -> Int -> [Char]
insertAt c xs 0 = c:xs
insertAt c (x:xs) p = x : (insertAt c xs (p-1))

borrar :: Linea -> Linea
borrar (xs, p) = (del xs p, p-1)

del :: [Char] -> Int -> [Char]
del [] _ = []
del (x:xs) 0 = xs
del (x:xs) p = x: (del xs (p-1))
