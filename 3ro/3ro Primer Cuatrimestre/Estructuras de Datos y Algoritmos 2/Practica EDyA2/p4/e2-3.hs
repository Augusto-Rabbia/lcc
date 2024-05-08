data BST a = E | N (BST a) a (BST a) deriving Show
max2 :: Ord a => BST a -> a
max2 (N _ x E) = x
max2 (N _ _ r) = max2 r

min :: Ord a => BST a -> a
min (N _ x E) = x
min (N l _ _) = min l


checkBST :: Ord a => BST a -> Bool
checkBST E = True
checkBST (N l x r) =
    if (min l) <= x && x <= (max2 r) then
        (checkBST l) && (checkBST r)
    else False

-- Ej3

member :: Ord a => a-> a-> BST a -> Bool
member x b E = x == b
member x b (N l v r) | x <= v = (member x v r)
                     | x > v = (member x b l)
