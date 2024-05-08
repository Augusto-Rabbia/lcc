-- 2020 dic
--  Ej 10)
filterInd p s = fIAux 0 p s

fIAux n p E = E
fIAux n p (L x) = if (p x) then (L (n,x)) else E
fIAux n p (N s l r) =
    let (l',r') = (fIAux n p l) ||| (fIAux (n+(size l)) p l)
    in (N ((size l') + (size r')) l' r') 

size E = 0
size (L x) = 1
size (N sz _ _ _) = sz

    
-- 2021 marz
-- Ej 5)
entradas s x =
    let
        s' = tabulateS (\i -> (false, i, (nthS s i))) len
        s'' = scanS (false, -2, 0) f s'
        s''' = appendS (fst s'') (singletonS (snd s''))
        (noDcto, dcto) = reduceS h (0,0) (tabulateS (\i -> (g (nthS s''' i))) len)
    in ((noDcto + (dcto/2))*x,dcto)
        where len = (lengthS s)

f _ (true,j,m) = (true,m)
f (_,i,n) (_,j,m) = if (n < m) && ((i+1) == j) then (true,j,m)

-- obtiene (cantNoDescuento, cantDescuento)
g (b,_,n) = if b then (0,n) else (n,0)
-- Suma el total de cada tipo de ticket
h (n,m) (n',m') = (n+n',m+m')

-- 2019
--  Ej 1) ESTA MAL
aumentos s = reduce maxAum ('a',0,(maxBound :: Int)) (aumDyC s (map toInfo s))

aumDyC s infos = case showtS of
                    EMPTY = ('e',0,0) -- no debería ocurrir
                    ELT x = map (\y -> addInfo (toInfo x) y) infos  
                    NODE l r = 


-- mercadería y precio a (merc, cant_aumentos, precio_actual)
toInfo (m, p) = (m,0,p)

-- Ej 2)
names [] = []
names ((File n _):xs) = n:(names xs)
names ((Dir n _):xs) = n:(names xs)


mkdir [] n (Dir name subd) = if (inList n (names subd)) then (Dir name subd)
                    else (Dir name ((Dir n []):subd))
mkdir ("/":ps) n (Dir name subd) = if name == "/" then mkdir ps n (Dir name subd)
                                                  else (Dir name subd)
mkdir (p:ps) n (Dir name subd) = let (dir,rest) = retDir p subd
                                 in if (dirName dir) == [] then (Dir name subd)
                                                 else (Dir name ((mkdir ps n dir):rest))


dirName (Dir n _) = n

retDir _ [] = ((Dir [] []),[])
retdir nam ((File n t):ds) = let (d,ds') = retDir nam ds
                                in (d,(File n t):ds')
retDir nam ((Dir n subd):ds) = if nam == n then ((Dir n subd),ds)
                                    else let (d,ds') = retDir nam ds
                                        in (d,(Dir n subd):ds') 

-- Mi 2do parcial
toInfo i s n = if (nthS s i) > n then (1, i, i) 
                    else (0, i, i)

f (m, i, j) (m', k, l) = if j+1 == l && m' > 0 
                            then (m+m', i, l)
                            else (m', k, l) 

maxStreak n s = let s' = tabulateS (\i -> (toInfo i s n)) (lengthS S) 
                    (a, b) = scanS f s'
                    s'' = appendS (drop 1 a) (singletonS b)
                    g (x,_,_) = x
                    s''' = map g s''  
                    in reduceS max 0 s'''