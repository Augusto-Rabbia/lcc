{-    1)
a- Test :: (Int -> Int) -> Int -> Bool
b- esMenor :: Ord a => a -> a -> Bool
c- eq :: Eq a => a -> a -> Bool
d- showVal :: Show a => a -> [Char] (string)

    2)
a- (+5) :: Num a => a -> a
    Suma 5 a un valor numerico
b- (0<) :: Ord a => a -> Bool
    Determina si un valor ordenable es mayor a 0
c- ('a':) :: [Char] -> [Char] (???)
    Coloca el caracter a antes de un String
d- (++"\n") :: [Char] -> [Char]
    Appendea el string "\n" en frente de un String
e- filter (≡ 7) :: (Num a, Eq a) => [a] -> [a]
    Devuelve unaaplicarFunc lista con todos los 7 en una lista con valores numericos que puedan ser comparados con igualdad
f- map (++[1]) :: [[a]] -> [[a]]
    Appendea el numero 1 a todas las sublistas dentro de una lista de listas
-}
--    3)
-- a  
aplicarEnDos :: (Int -> Int) -> Int
aplicarEnDos f = f 2
aplicarEnYMultPorDos :: (Int -> Int) -> Int
aplicarEnYMultPorDos f = (f 2) * 2
-- b  
sumarN :: Int -> (Int -> Int)
sumarN n = (\x -> n + x)
multiplicarPorN :: Int -> (Int -> Int)
multiplicarPorN n = (\x -> n + x)
-- c
aplicarFunc :: (Int -> Int) -> (Int -> Int) 
aplicarFunc f = \x -> f x
aplicarFuncPorArg :: (Int -> Int) -> (Int -> Int)
aplicarFuncPorArg f = \x -> (f x) * x
-- d
(0<) :: Int -> Bool
(5==) :: Int -> Bool
-- e
andear :: Bool -> (Bool -> Bool)
andear a = \x -> x && a


-- f
-- Al parecer la tuplas (Int, Char) representan código Unicode
-- (Código, Caracter)
esMayuscula :: (Int,Char) -> Bool 
esMayuscula (cod, car) = codigo >= 65 && codigo <= 90
esDigitoPar :: (Int, Char) -> Bool
esDigitoPar (dig, _) -> even dig
-- g
multTupla :: (Int, Int) -> Int
multTupla (a,b) = a*b
sumarTupla :: (Int, Int) -> Int
sumarTupla (a,b) = a+b
-- h
tuplaNN :: Int -> (Int,Int)
tuplaNN n = (n,n)

-- i
:: a -> Bool
-- j


--      4)
-- a
-- if true then false else true where false = True; true = False
-- No anda el where?
-- b
-- if if then then else else 
-- Error de tipos
-- c
False == (5 >= 4)
-- Devuelve False
-- d
-- 1 + if ('a' < 'z') then -1 else 0
-- Devuelve 0
-- e
-- if fst p then fst p else snd p where p = (True,2)
-- No anda el where?
-- g No anda el where?

--      5)
-- a
-- f x = let (y,z) = (x,x) in y
f x = x
-- b
-- greater (x,y) = if x > y then True else False
greater (x,y) = x > y
-- c
f (x,y) = let z = x + y in g (z,y) where g (a,b) = a-b
-- Esta funcion hace g (x+y,y) = x+y-y = x, osea, devuelve el primer arg
f (x,y) = x
--      6)
-- a
smallest = \ (x,y,z) -> if x < y && x < z then x else if y < x && y < z then y else z
-- equivalente a
smallest (x,y,z) = if x < y && x < z then x else if y < x && y < z then y else z
-- b
second = \x -> x
-- c
andThen = \x -> \y -> if x then y else False
-- parece estar limitada a que y sea un bool por alguna razon
-- PQ ES UNA FUNCION QUE DEVUELVE y O False, AMBOS TIENEN QUE SER BOOL ENTONCES
-- d
twice = \f -> \x -> f (f x)
-- e
flip = \f -> \x -> \y -> f y x
-- f
inc = \x -> x+1
--      7)
iif x y = if x then not y else y
alpha x = x
--      8)
{- h:: c -> d la tercera es la equivalente
-}      
--      9)
myZip3 :: [a] -> [b] -> [c] -> [(a, b, c)]
myZip3 [] _ _ = []
myZip3 _ [] _ = []
myZip3 _ _ [] = []
myZip3 (x:xs) (y:ys) (z:zs) = (x,y,z) : zip3 xs ys zs

myZip3' :: [a] -> [b] -> [c] -> [(a, b, c)]
myZip3' xs ys zs = [(x,y,z) | ((x,y),z) <- zip (zip xs ys) zs]
-- myZip3' [1,2] [3,4] [5,6]
--      10)
--a F [[]] ++ xs = xs ---> []: xs
-- Tiene sentido si xs es una lista de listas, 
--b F [[]] ++ xs = [xs] ---> []: xs
--c V
--d V
--e V
--f F
--g V
--h V
--i V
--j V
--      11)


--      12)


--      13)


--      14)
