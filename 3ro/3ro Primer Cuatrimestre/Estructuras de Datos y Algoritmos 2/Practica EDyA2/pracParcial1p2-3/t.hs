
-- Ej 9

myZip3 :: [a] -> [b] -> [c] -> [(a, b, c)]
myZip3 [] _ _ = []
myZip3 _ [] _ = []
myZip3 _ _ [] = []
myZip3 (x:xs) (y:ys) (z:zs) = (x, y, z) : zip3 xs ys zs

myZip3' :: [a] -> [b] -> [c] -> [(a, b, c)]
myZip3' xs ys zs = [(x, y, z) | (x, (y, z)) <- zip xs (zip ys zs)]

-- Ej13
divisors x = [z | z <- [1..x], x `mod` z == 0]
matches x xs = [z | z <- xs, z == x]
cuadruplas n = [(a,b,c,d) | a <- [1..n],
                            b <- [1..n],
                            c <- [1..n],
                            d <- [1..n],
                            a /= c || b /= d || (a == b && a == c && a == d),
                            a^2 + b^2 == c^2 + d^2]
unique [] = []
unique (x:xs) = x: [z | z <- unique xs, x /= z]

-- Ej14
scalprod xs ys = sum [x*y | (x,y) <- zip xs ys]