{- Ejercicio 2 (Tema 1):
Dada la representación de pares y listas en Sistema F: 
    Pair X Y = ∀Z. (X -> Y -> Z) -> Z
    List X = ∀Y. (X -> Y -> Y) -> Y -> Y
Suponiendo que se tienen definidas les funcionen cons, nil, pair, snd y fst, definir la siguiente fiinción en sistema F: 
    unZipF: ∀X. ∀Y. ListF (Pair X Y) -> Pair (ListF X) (ListF Y)
que transforma mm lista de pares en un par de listas. 
Por ejemplo, 
    unZipF ([(x1, y1), (x2, y2), (x3, y3)]) = ([x1, y2, x3],[y1, y2, y3]) 
-}

unZipF = ΛX. ΛY. \l: ListF (Pair X Y). pair<List X, List Y> 
                                            (l<List X> (\x:Pair X Y. \xs: List X. cons<X> (fst x) xs) Nil<List X>) 
                                            (l<List Y> (\x:Pair X Y. \xs: List Y. cons<Y> (snd x) xs) Nil<List Y>) 



{- Ejercicio 3 (Tema 1): 
Una forma alternativa de definir una mónada en Haskell es a través de la siguiente clase: 
    class Kleisli m where 
        (>=>) :: (a -> m b) -> (b -> m c) -> (a -> m c)
        ret :: a -> m a 

donde el operador >=> es llamado Kleisli composition.
En esta formulación las leyes son más fáciles de expresar: 
    (f >=> g) >=> h = f >=> (g >=> h)   -- asociatividad 
    ret >=> f       = f                 -- neutro a izquierda 
    f >=> ret       = f                 -- neutro a derecha
-}
-- a) Dar la instancia Kleisli para el tipo de dates State a donde: 
--    newtype State s a = St {runSt :: s -> (s, a)}

instance Kleisli (State a) where
    ret x = St (\s -> (s,x))
    f >=> g = \a -> 
                St (\s -> let (St sb) = f a
                              (s',b) = sb s
                            in runSt (g b) s')

-- b) Mostrar que si un constructor m es una instancia de Monad, m es una instancia de Kleisli, es decir completar la siguiente definición: 
--    instance Monad m Kleisli m where ret = ; f >=> g = 

instance Monad m Kleisli m where 
    ret x = return x
    f >=> g = (\x -> f x >>= g)

-- c) Probar que si se satisfacen las leyes de las mónadas también se satisfacen las leyes de Kleisli. 

(Kleisli.1) (f >=> g) >=> h = f >=> (g >=> h)
(f >=> g) >=> h                 = {def >=>}
(\x -> f x >>= g) >=> h         = {def >=>}
\x -> (\x -> f x >>= g) x >>= h = {app}
\x -> (f x >>= g) >>= h         = {monad.3}
\x -> f x >>= (\x. g x >>= h)   = {def >=>}
f >=> (\x. g x >>= h)           = {def >=>}
f >=> (g >>= h)

(Kleisli.2) ret >=> f = f
ret >=> f           = {def >=>}
(\x -> ret x >>= f) = {monad.1}
(\x -> f x)         =
f

(Kleisli.2) f >=> ret = f
f >=> ret           = {def >=>}
(\x -> f x >>= ret) = {monad.2}
(\x -> f x)         =
f


{- Ejercicio 3 Tema 2:
Una forma alternativa de definir una mónada en Haskell es a través de la siguiente clase: 
    class Functor t => Triple t where
        ret :: a -> a
        join :: t (t a) -> t a 

Las leyes que deben satisfacer las instancias son las siguientes:
    join . ret          = id                    -- L1 
    join . fmap ret     = id                    -- L2 
    join . fmap join    = join . join           -- L3 
    join . (fmap (fmap f)) = (fmap f) . join    -- L4 
    ret . f             = fmap f . ret          -- L5
-}

-- a) Dar las instancias de Maybe y de [] para Triple. No hace falta probar que se satisfacen las leyes. 

instance Triple Maybe where
    ret x = Just x
    join (Just (Just x)) = (Right x)
    join (Just Nothing) = Nothing
    join Nothing = Nothing

instance Triple [] where
    ret x = [x]
    join [] = []
    join (xs:xss) = xs ++ (join xss) -- join xs = concat xs

-- b) Probar que toda instancia de Triple es una mónada, para ello completar la definición 
--      y probar que si se satisfacen las leyes de Triple, se satisfacen las leyes de la mónada. 
 
instance Triple t => Monad t where 
    return x = ret x 
    p >>= q = join (fmap q p)


