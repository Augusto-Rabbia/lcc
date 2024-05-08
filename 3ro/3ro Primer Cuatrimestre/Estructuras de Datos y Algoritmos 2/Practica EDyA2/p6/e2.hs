data Tree a = E | Leaf a | Join (Tree a) (Tree a)

-- a)
-- Con mapReduce se haría: (ESTO NO CONSIDERA LOS NODOS EMPTY IGUAL!!!) 
toInfo v = (v', v', v', v)
            where v' = (max v 0)
mcss s = mapReduce toInfo f (0,0,0,0) s

f (m, p, s, t) (m', p', s', t') = (max m m' (s + p'),
                                   max p (t+p'),
                                   max s' (s+t')
                                   t+t')

--trabajo: O(n) + O(SUM (f x)) (toInfo es O(1) así que no afecta)
--prof: O(lg n) + O(lg n * MAX (f x))

-- Con D&C
mcss s = fst (mcssDC s)

mcssDC E = (0,0,0,0)
mcssDC Leaf x = (x',x',x',x)
                    where x' = max x 0
mcssDC (Join l r) = (f l' r')
                    where (l',r') = (mcssDC l) ||| (mcssDC r)
                
-- Mismo trabajo y profundidad. O(n) cant nodos, profundidad es O(h) con h altura, o sea O(lg n)
