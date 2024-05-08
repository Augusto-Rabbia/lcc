divisores :: Int -> [Int]
divisores x | x <= 0 = []
            | otherwise = [y | y <- [1 .. x], x `mod` y == 0]

matches :: [Int] -> Int -> [Int]
matches xs y | xs == [] = []
             | otherwise = [z | z <- xs, z == y]

cuadruplas :: Int -> [(Int, Int, Int, Int)]
cuadruplas n = [(a,b,c,d) | a <- [1 .. n], 
                            b <- [1 .. n], 
                            c <- [1 .. n], 
                            d <- [1 .. n],
                            a /= c,
                            b /= d, 
                            a^2+b^2 == c^2+d^2]

unique :: [Int] -> [Int]
unique []  = []
unique (x:xs) = x : [y | y <- unique xs, x /= y]
