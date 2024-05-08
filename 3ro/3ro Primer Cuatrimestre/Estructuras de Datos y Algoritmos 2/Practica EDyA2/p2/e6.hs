smallest :: Ord a => (a,a,a) -> a
smallest = \(x, y, z) -> 
    if x <= y && x <= z then x 
    else if y <= z then y
    else z

second = \x -> \y -> y

andThen = \x -> \y -> if x then y else False

twice = \f -> \x -> f (f x)

flip' = \f -> \x -> \y -> f y x

inc = \x -> (+1) x