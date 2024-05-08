scalprod :: [Int] -> [Int] -> Int
scalprod xs ys = sum [x*y | (x,y) <- (zip xs ys)]
