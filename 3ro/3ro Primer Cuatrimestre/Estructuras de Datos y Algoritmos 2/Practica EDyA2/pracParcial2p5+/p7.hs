-- Ej 1)


-- Ej 2)

-- Ej 3)

-- Ej 4)
data Paren = Open | Close

matchParen :: Seq Paren -> Bool
matchParen s = (matchP s) == 0

matchP s = case (showtS s) of
            EMPTY = (0,0)
            ELT x = if x == Open then (1,0) else (0,1)
            NODE l r = 
                    (a,b) = (matchP l) ||| (matchP r) 

combine (o,c) (o',c') = if o < c' then (o,c+c'-o)
                                  else (o+o'-c',c)

-- Ej 5) MCSS
