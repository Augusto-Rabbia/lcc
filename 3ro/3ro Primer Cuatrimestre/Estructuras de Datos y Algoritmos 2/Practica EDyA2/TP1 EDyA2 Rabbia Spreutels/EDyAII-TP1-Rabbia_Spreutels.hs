-- Manuel Spreutels
-- Augusto Rabbia

import Data.List

data NdTree p = N (NdTree p) p (NdTree p) Int | E deriving (Eq,Ord,Show)

class Punto p where
  dimension :: p ->Int
  coord :: Int -> p -> Double
  dist :: p -> p -> Double

  dist p q = sum [((coord k p) - (coord k q))^2 | k <- [0..dimension p - 1]]  


newtype Punto2d = P2d (Double,Double) 
newtype Punto3d = P3d (Double,Double,Double)

instance Punto Punto2d where
  dimension _ = 2
  coord k (P2d (x,y))
    | k == 0 = x
    | otherwise = y

instance Punto Punto3d where
  dimension _ = 3
  coord k (P3d (x, y, z))
    | k == 0 = x
    | k == 1 = y
    | otherwise = z

instance Show Punto2d where
  show (P2d (x,y)) = "(" ++ show x ++ "," ++ show y ++ ")"

instance Eq Punto2d where
  (==) p q = and [(coord k p) == (coord k q) | k <- [0..dimension p - 1]] 

instance Show Punto3d where
  show (P3d (x,y,z)) = "(" ++ show x ++ "," ++ show y ++ "," ++ show z ++ ")"

instance Eq Punto3d where
  (==) p q = and [(coord k p) == (coord k q) | k <- [0..dimension p - 1]] 



-- Dado un eje y una lista de puntos, son ordenados de menor a mayor
-- según la componente correspondiente
ordenarPorEje :: Punto p => [p] -> Int -> [p]
ordenarPorEje l eje = sortBy (\x y -> compare (coord eje x) (coord eje y)) l

-- Dada una lista, devuelve el elemento del medio si tiene longitud impar,
-- y el central de la derecha si es par, y las listas que
-- se encuentran a su derecha e izquierda
divide :: [a] -> ([a], a, [a])
divide l = 
  let 
    len = (length l)
    index = (div len 2)
  in if (len `mod` 2) == 0 then ((take index l), (l !! index), (drop (index+1) l))
     else ((take index l), (l !! index), (drop (index+1) l))


-- Construye un NdTree balanceado a partir de una lista de Punto
fromList :: Punto p => [p] -> NdTree p
fromList [] = E
fromList ps = fromListAux ps 0 (dimension (head ps))
  
-- Construye un NdTree balanceado a partir de una lista de Punto.
-- Con argumentos necesarios para la recursión
fromListAux :: Punto p => [p] -> Int -> Int -> NdTree p
fromListAux [] _ _ = E
fromListAux ps eje dimension = (N izquierda medio derecha eje) where
              ordenada = ordenarPorEje ps eje
              nuevo_eje = ((eje + 1) `mod` dimension)
              (izquierda', medio, derecha') = divide ordenada
              izquierda = fromListAux izquierda' nuevo_eje dimension
              derecha = fromListAux derecha' nuevo_eje dimension

-- Inserta un Punto en un NdTree, donde corresponde
insertar :: Punto p => p -> NdTree p -> NdTree p
insertar p E = N E p E 0
insertar p tree = insertarAux p tree 0 (dimension p)


-- Inserta un Punto en un NdTree, donde corresponde
insertarAux :: Punto p => p -> NdTree p -> Int -> Int -> NdTree p
insertarAux p E eje _ = (N E p E eje)
insertarAux p (N l p1 r e) eje dim = if (coord eje p) <= (coord eje p1) 
                              then (N (insertarAux p l ((eje+1) `mod` dim) dim) p1 r e)
                              else (N l p1 (insertarAux p r ((eje+1) `mod` dim) dim) eje)
                              
-- Elimina un Punto de un NdTree, manteniendo el 
-- invariante respecto de los ejes
eliminar :: (Eq p, Punto p) => p -> NdTree p -> NdTree p
eliminar p E = E
eliminar p t@(N E p1 E e) = if p == p1 then E else t
eliminar p t@(N l p1 E e) | (coord e p) > (coord e p1) = t
                          | (coord e p) < (coord e p1) = (N (eliminar p l) p1 E e)
                          | p == p1 = (N (eliminar reemplazo l) reemplazo E e)
                          | otherwise = (N (eliminar p l) p1 E e)
                         where
                          reemplazo = (maxEje l e)
eliminar p (N l p1 r e) | (coord e p) > (coord e p1) = (N l p1 (eliminar p r) e)
                        | (coord e p) < (coord e p1) = (N (eliminar p l) p1 r e)
                        | p == p1 = (N l reemplazo (eliminar reemplazo r) e)
                        | otherwise = (N (eliminar p l) p1 r e)
                       where
                         reemplazo = (minEje r e)

-- Determina el Punto con valor máximo en una componente
-- dentro de un NdTree
maxEje :: Punto p => NdTree p -> Int -> p
maxEje (N E p E e) eje = p
maxEje (N E p r e) eje =  if e == eje then maxEje r eje
                          else let 
                            p1 = maxEje r eje
                          in if (coord eje p1) <= (coord eje p) 
                             then p else p1
maxEje (N l p E e) eje = if e == eje then p 
                         else let                           
                            p1 = maxEje l eje
                          in if (coord eje p1) <= (coord eje p) then p else p1
maxEje (N l p r e) eje = if e == eje then maxEje r eje
                         else let 
                            p1 = maxEje l eje
                            p2 = maxEje r eje
                          in (max3p eje p1 p2 p)

-- Obtiene el Punto con valor máximo en una componente dada
-- entre 3 elementos de clase Punto
max3p :: Punto p => Int -> p -> p -> p -> p
max3p eje p1 p2 p3 | coord2 <= coord1 && coord3 <= coord1 = p1
                   | coord1 <= coord2 && coord3 <= coord2 = p2 
                   | otherwise = p3
                  where
                    coord1 = (coord eje p1)
                    coord2 = (coord eje p2)
                    coord3 = (coord eje p3)

-- Determina el Punto con valor mínimo en una componente
-- dentro de un NdTree
minEje :: Punto p => NdTree p -> Int -> p
minEje (N E p E e) eje = p
minEje (N E p r e) eje =  if e == eje then p
                          else let 
                            p1 = minEje r eje
                          in if (coord eje p1) <= (coord eje p) then p1 else p
minEje (N l p E e) eje = if e == eje then minEje l eje
                         else let                           
                            p1 = minEje l eje
                          in if (coord eje p1) <= (coord eje p) then p1 else p
minEje (N l p r e) eje = if e == eje then minEje l eje
                         else let 
                            p1 = minEje l eje
                            p2 = minEje r eje
                          in (min3p eje p1 p2 p)

-- Obtiene el Punto con valor mínimo en una componente dada
-- entre 3 elementos de clase Punto
min3p :: Punto p => Int -> p -> p -> p -> p
min3p eje p1 p2 p3 | coord1 <= coord2 && coord1 <= coord3 = p1
                   | coord2 <= coord3 && coord2 <= coord1 = p2 
                   | otherwise = p3
                  where
                    coord1 = (coord eje p1)
                    coord2 = (coord eje p2)
                    coord3 = (coord eje p3)

type Rect = (Punto2d,Punto2d)

-- Dada una región y un Punto2d, determina si el punto
-- pertenece a la región
inRegion :: Punto2d -> Rect -> Bool
inRegion p region = inRegionOrdenado p (reordenar region)

-- Dada una región y un Punto2d, determina si el punto
-- pertenece a la región, dado que esta está ordenada.
inRegionOrdenado :: Punto2d -> Rect -> Bool
inRegionOrdenado p (p1,p2) | (coord 0 p) < (coord 0 p1) = False
                           | (coord 0 p) > (coord 0 p2) = False
                           | (coord 1 p) < (coord 1 p1) = False
                           | (coord 1 p) > (coord 1 p2) = False
                           | otherwise = True

-- Devuelve una región equivalente a la dada, pero ordenada. 
-- Diremos que una región está ordenada si está expresada en
-- función de sus esquinas inferior izquierda y superior derecha
-- Esto para simplificar los calculos de inRegion y ortogonalSearch
reordenar :: Rect -> Rect
reordenar ((P2d (x1,y1)), (P2d (x2,y2))) = ((P2d ((min x1 x2), (min y1 y2))), (P2d ((max x1 x2), (max y1 y2))))

-- Dado un árbol de Punto2d y una región, devuelve una lista
-- de todos los puntos del árbol que se encuentran en la región
ortogonalSearch :: NdTree Punto2d -> Rect -> [Punto2d]
ortogonalSearch tree rect = ortogonalSearchAux tree (reordenar rect) 

-- Dado un árbol de Punto2d y una región, devuelve una lista
-- de todos los puntos del árbol que se encuentran en una región ordenada.
ortogonalSearchAux :: NdTree Punto2d -> Rect -> [Punto2d]
ortogonalSearchAux E _ = []
ortogonalSearchAux (N l p r e) (p1,p2)
  | (inRegionOrdenado p (p1,p2)) = p:((ortogonalSearchAux l (p1,p2)) ++ (ortogonalSearchAux r (p1, p2)))
  | e == 0 && x < x1 = ortogonalSearchAux r (p1,p2)
  | e == 0 && x > x2 = ortogonalSearchAux l (p1,p2)
  | e == 1 && y < y1 = ortogonalSearchAux r (p1,p2)
  | e == 1 && y > y2 = ortogonalSearchAux l (p1,p2)
  | otherwise = ((ortogonalSearchAux l (p1,p2)) ++ (ortogonalSearchAux r (p1,p2)))
    where
      x = (coord 0 p)
      y = (coord 1 p)
      x1 = (coord 0 p1)
      y1 = (coord 1 p1)
      x2 = (coord 0 p2)
      y2 = (coord 1 p2)
