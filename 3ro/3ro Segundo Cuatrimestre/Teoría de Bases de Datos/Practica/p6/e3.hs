{-
Escribir un programa interactivo que implemente un juego en el que hay que adivinar un número secreto predefinido.
El jugador ingresa por teclado un número y se le responde si el número es mayor, menor, o igual al número secreto.
Si el jugador adivina el número, el juego termina.
Ayuda: para convertir una String en Int puede usar la función read :: String → Int.
-}

main :: IO ()
main = do putStrLn "Adivina el numero"
          juego 5

juego :: Int -> IO ()
juego n = do guess <- getLine
             let g = (read guess)
             if g == n
                then putStrLn "Ganaste"
                else if g >= n
                     then do putStrLn "Alto"
                             juego n
                     else do putStrLn "Bajo"
                             juego n 
