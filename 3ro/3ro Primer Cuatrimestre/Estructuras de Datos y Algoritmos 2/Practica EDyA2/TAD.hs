vacia :: [a]
vacia = []

esVacia :: [a] -> Bool
esVacia [] = True
esVacia (x:xs) = False

poner :: a -> [a] -> [a]
poner x xs = x:xs

primero :: [a] -> a
primero (x:[]) = x
primero (x:xs) = primero xs

sacar :: [a] -> [a]
sacar (x:[]) = vacia
sacar (x:xs) = x:(sacar xs)

