merge :: (a → a → Ordering) → Seq a → Seq a → Seq a
merge f s1 s2 = case (showtS s1) of
                    EMPTY = s2
                    ELT x = let (l', r') = splitAt f s2 x
                            in appendS l' (appendS (singletonS x) r')
                    NODE l r = let (l', r') = splitAt s2 (nthS r 0)
                               in 


splitAt :: Ord a => (a -> a -> Bool) -> Seq a -> a -> (Seq a -> Seq a)
splitAt f s x = case (showtS s1) of
                    EMPTY = (emptyS, emptyS)
                    ELT y = if (f x y) then (appendS (singletonS x) (singletonS y))
                    NODE l r = if (f x (nthS r 0)) then 
