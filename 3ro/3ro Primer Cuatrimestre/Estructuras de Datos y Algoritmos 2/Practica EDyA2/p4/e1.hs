data Tree a = E | N (Tree a) a (Tree a) deriving Show

completo :: a -> Int -> Tree a
completo x 0 = E
completo x n = (N t x t) where
            t = (completo x (n-1))

--xd
balanceado :: a -> Int -> Tree a
balanceado x 0 = E
balanceado x n = (N t x t) where
    t = (completo x (n-2))
