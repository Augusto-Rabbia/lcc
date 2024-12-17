{-
Dar una definición de la función 
    getChars:: Int → IO String
que dado n lea n caracteres del teclado, usando las funciones sequenceA y replicate.
-}
-- Considerar:
{-
replicate :: Int -> a -> [a]
replicate 0 _ = []
replicate n x = x : replicate (n - 1) x
-}
{-
sequenceA :: (Applicative f) => [f a] -> f [a]
sequenceA [] = pure []
sequenceA (x:xs) = pure (:) <*> x <*> sequenceA xs
-}

getChars :: Int -> IO String
getChars n = sequenceA (replicate n getChars)

