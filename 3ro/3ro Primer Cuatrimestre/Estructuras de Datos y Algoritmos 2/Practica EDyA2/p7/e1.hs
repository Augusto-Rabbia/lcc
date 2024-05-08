{- Implementación del TAD secuencia -}

module Seq where

class Seq s where
   emptyS     :: s a
   singletonS :: a -> s a
   lengthS    :: s a -> Int 
   nthS       :: s a -> Int -> a 
   tabulateS  :: (Int -> a) -> Int -> s a
   mapS       :: (a -> b) -> s a -> s b 
   filterS    :: (a -> Bool) -> s a -> s a 
   appendS    :: s a -> s a -> s a
   takeS      :: s a -> Int -> s a
   dropS      :: s a -> Int -> s a
   showtS     :: s a -> TreeView a (s a)
   showlS     :: s a -> ListView a (s a)
   joinS      :: s (s a) -> s a
   reduceS    :: (a -> a -> a) -> a -> s a -> a
   scanS      :: (a -> a -> a) -> a -> s a -> (s a, a)
   fromList   :: [a] -> s a


promedios :: Seq Int -> Seq Float
promedios s =  promAux (appendS a (singletonS b)) (lengthS s)
               where (a, b) = (scanS (+) 0 s)

promAux s 1 = s
promAux s n = appendS (promAux s (n-1)) (singletonS ((nthS s n) / n))
-- Es mejor con tabulateS, que haga exactamente esto

mayores :: Seq Int -> Int
mayores s = mayoresAux (appendS a (singletonS b))
            where (a, b) = (scanS max (nthS s 0) s) 

mayoresAux s | (lengthS s) == 1 = 0
             | otherwise = if (nthS 0 s) < (nthS 1 s) then 1 + (mayoresAux (dropS s 1))
                              else (mayoresAux (dropS s 1))
