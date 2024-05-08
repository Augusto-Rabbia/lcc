-- sccml Divide and Conquer
-- sccml : Seq Int -> Int
{-Info = (Int sccml de la subSeq
        Int primer elemento de la subSeq
        Int ultimo ''
        Int len prefijo creciente más largo
        Int len sufijo ''
        Int len de la subSeq)
-}
type Info = (Int,Int,Int,Int,Int,Int)

sccml :: Seq s => s Int -> Int
sccml s = (\(sc,_,_,_,_,_) -> sc-1) (sccmlDC s)

sccmlDC :: Seq s => s Int -> Info
sccmlDC s = case showtS s of
                EMPTY -> (0,0,0,0,0,0)
                ELT x -> x
                (NODE l r) -> let (l', r') = (sccmlDC l) ||| (sccmlDC r)
                            in (combineDC l' r')

combineDC :: Info -> Info -> Info
combineDC (sc,fst,lst,pre,suf,len)
          (sc',fst',lst',pre',suf',len') =
            let m = if lst < fst' then suf+pre' else 0
                nuevoPre = if m != 0 && len == pre then pre + pre' else pre
                nuevoSuf = if m != 0 && len == suf' then suf' + suf else suf'   
            in (maxscc (max sc' m),fst,ult',nuevoPre,nuevoSuf,len+len')


--  b)
-- Guardamos intervalos de valores que representan 
-- ir desde i-ésimo lugar en la Seq: ((_,i) , _)
-- hasta el j-ésimo:                 (_, (_,j))
-- T.q. el primer y último valor son ((primer,_), (ultimo,_))
type Intervalo a = ((a,Int), (a,Int))
-- index s convierte cada valor de s a un intervalo de tamaño 1.
index s = tabulateS (\i -> ((nthS s i, i), (nthS s i, i))) (lengthS s)

-- Combine inter combina los intervalos si el sufijo del primero
-- Es menor al del segundo. Así dando una subsecuencia creciente contigua
combineInter :: Num a -> Intervalo a -> Intervalo a -> Intervalo a
combineInter ((l1, i1), (r1,j1))
             ((l2, i2), (r2,j2)) = if r1 < l2 && j1 == (i2-1)  
                                      then ((l1,i1), (r2,j2)) 
                                      else ((l2,i2), (r2,j2))

-- s' es s mapeado a intervalos de longitud 1 (bien podría usarse map) (mentira, map no podria llevar constancia del numero)
-- f devuelve la longitud de un intervalo
-- g appendea la primer y segunda parte del scan
-- casoB es el caso base, 
sccml s = let s' = index s
              f ((_,i), (_,j)) = j - i
              g (s, r) = appendS s (singletonS r)
              casoB = (((nthS s 0),0), (maxBound,0))
              s'' = mapS f (g (scan combineInter casoB s'))
          in reduce max 0 s''




