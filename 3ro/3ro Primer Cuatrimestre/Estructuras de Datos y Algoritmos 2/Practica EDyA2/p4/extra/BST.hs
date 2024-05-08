data BST a = E | N (BST a) a (BST a)

isInBST :: Ord a => a -> BST a -> Bool
isInBST _ E = False 
isInBST x (N l y r) | x == y = True
                    | x < y = isInBST x l
                    | otherwise = isInBST x r
-- Sin embargo, existe una implementación potencialmente más eficiente, 
-- que hace h comparaciones en vez de 2*h en el peor caso
isInBST x E = False  
isInBST x t@(N l y r) = isInBSTAux y x t
-- El primer x es la mejor aproximacion a x en el BST
isInBSTAux z x E = x == z
isInBSTAux z x (N l y r) | x <= y = isInBSTAux y x l
                         | otherwise = isInBSTAux z x r

insert :: Ord a => a -> BST a -> BST a
insert x E = (N E x E)
insert x (N l y r) | x <= y = (N (insert x l) y r)
                   | otherwise = (N l y (insert x r))

delete x E = E
delete x (N l y r) | x < y = delete x l
                   | x > y = delete x r
                   | x == y = if l == E then
                                if r == E then E
                                    else r  
                                else if r == E then l
                                    else let max = maxVal l
                                         in (N (delete max l) max)

maxVal (N _ y E) = y
maxVal (N _ _ r) = maxVal r