-- Los RBT son BSTs pero balanceados (con altura O(lg n)), al igual que los AVL, pero con una impl más sencilla. 
-- Lo logran manteniendo 2 invariantes:
-- INV 1: Ningún Nodo rojo tiene un hijo rojo
-- INV 2: Todos los caminos de la raíz a una hoja tienen el mismo número de nodos negros (altura negra)
-- Entonces, el camino más largo es a lo sumo 2 veces el camino más corto.

data Color = R | B
data RBT a = E | N Color (RBT a) a (RBT a)

-- la función isInRBT es igual que para BST, se ignora el color.
-- Es medio compleja XD porque necesita balancearse y etc
insertRBT :: Ord a => a -> RBT a -> RBT a
insertRBT x E = (N R E x E)
insertRBT x (N l y r) | x < y = insertRBT x l
                      | x > y = insertRBT x r
                      | otherwise = 
