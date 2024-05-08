data T a = E | N (T a) a (T a) deriving Show
altura :: T a -> Int
altura E = 0
altura (N l x r) = 1 + (max (altura l) (altura r))

combinar :: T a -> T a -> T a
combinar E t2 = t2
combinar t1 E = t1
combinar t1 t2 = (N a b t2)
                where (a, b) = maxLeft t1

maxLeft :: T a -> (T a, a)
maxLeft (N E y r) = (r, y)
maxLeft (N (N E x r) y r') = ((N r y r'), x)
maxLeft (N l x r) = ((N a x r),b)
                    where (a,b) = maxLeft l

filterT :: (a -> Bool) -> T a -> T a
filterT _ E = E
filterT f (N l x r) = if (f x) then (N a x b) 
                               else (combinar a b)
                      where (a,b) = ((filterT f l), (filterT f r))

-- seguramente mal
quicksortT :: T Int -> T Int
quicksortT E = E
quicksortT (N l x r) = (N (quicksortT (filterT (<=x) ((N l (x+1) r)))) x (quicksortT (filterT (>x) ((N l (x-1) r)))))



