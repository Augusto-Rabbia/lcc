
myZip3 :: [a] -> [b] -> [c] -> [(a, b, c)]
myZip3 [] _ _ = []
myZip3 _ [] _ = []
myZip3 _ _ [] = []
myZip3 (x:xs) (y:ys) (z:zs) = (x,y,z) : zip3 xs ys zs