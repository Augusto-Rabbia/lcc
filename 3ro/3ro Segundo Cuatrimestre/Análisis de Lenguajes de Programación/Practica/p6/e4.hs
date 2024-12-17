{-
4. El juego nim consiste en un tablero de 5 filas numeradas de asteriscos. El tablero inicial es el siguiente:
1 : ∗ ∗ ∗ ∗ ∗
2 : ∗ ∗ ∗ ∗
3 : ∗ ∗ ∗
4 : ∗ ∗
5 : ∗

Dos jugadores se turnan para sacar una o más estrellas de alguna fila.
El ganador es el jugador que saca la última estrella de alguna fila.
Implementar el juego en Haskell.
Ayuda: para convertir una String en Int puede usar la función read :: String → Int.
-}

replacenList :: [a] -> a -> Int -> [a]
replacenList [] _ _ = []
replacenList (x:xs) y 0 = y:xs
replacenList (x:xs) y n = x : (replacenList xs y (n-1))

type Board = [Int]

data Jugador = Jug String

main :: IO ()
main = do putStrLn "Comienza el juego"
          turno <- nim (Jug "1") [5, 4, 3, 2, 1]
          let (Jug n) = turno
            in if n == "1"
                    then putStrLn "Gano el jugador 2"
                    else putStrLn "Gano el jugador 1"

nim :: Jugador -> [Int] -> IO Jugador
nim (Jug n) board = do putStrLn ("Turno del Jugador " ++ n ++ " de qué fila quieres sacar?\nTablero: " ++ (concat (map show board)))
                       fila <- getLine
                       if board !! (read fila) == 0
                            then return (Jug n)
                            else do putStrLn "Cuántas fichas deseas sacar?"
                                    fichas <- getLine
                                    let newBoard = replacenList board (max 0 ((board !! (read fila)) - (read fichas))) (read fila)
                                        in if n == "1" 
                                            then nim (Jug "2") newBoard
                                            else nim (Jug "1") newBoard
