{- Ejercicio 6:
Se desea modelar computaciones con un estado global s. Para esto se define el siguiente tipo de datos e instancia de mónada:
-}
newtype State s a = St {runState :: s → (a, s)}
instance Monad (State s) where
    return x = St (\s -> (x,s))
    (St h) >>= f = St (\s -> let (x,s') = h s
                            in runState (f x) s')

-- a) Probar que la instancia efectivamente define una mónada.
{-
(monad.1) return x >>= k = k x
return x >>= k                              = {def return}
St (\s -> (x,s)) >>= k                      = {def >>=}
St (\s -> let (x',s') = (\s -> (x,s)) s
            in runState (k x') s')          = {aplicacion}
St (\s -> let (x',s') = (x,s)
            in runState (k x') s')          = {let in}
St (\s -> runState (k x) s)                 = {(1)  (k x) = (\s' -> f s') para algun f :: s -> (a,s)}
St (\s -> (\s' -> f s') s)                  = {aplicacion}
St (\s -> f s)                              = {(1)}
k x

(monad.2) m >>= return = m
Sabemos que m = (St h) para algun h :: s -> (a,s)
(St h) >>= return                               = {def >>=}
St (\s -> let (x,s') = h s
            in runState (return x) s')          = {def return}
St (\s -> let (x,s') = h s
            in runState (St (\s -> (x,s))) s')  = {runState}
St (\s -> let (x,s') = h s
            in (\s -> (x,s)) s')                = {aplicacion}
St (\s -> let (x,s') = h s
            in (x,s))                           = {(x,s') = h s}
St (\s -> h s)                                  =
St h

(monad.3) m >>= (\x. k x >>= h) = (m >>= k) >>= h
(St f) >>= (\x. k x >>= h)                                                          = {def >>=}
St (\s -> let (x,s') = (f s) in runState ((\x. k x >>= h) x) s')                    = {aplicacion}
St (\s -> let (x,s') = (f s) in runState (k x >>= h) s')                            = {(1) k x = (St g)}
St (\s -> let (x,s') = (f s) in runState ((St g) >>= h) s')                         = {def >>=}
St (\s -> let (x,s') = (f s) 
            in runState (St (\r -> let (y,r') = (g r) in runState (g y) t')) s')    = {runState}
St (\s -> let (x,s') = (f s) 
            in (\r -> let (y,r') = (g r) in runState (g y) t') s')                  = {aplicacion}
St (\s -> let (x,s') = (f s) 
            in let (y,r') = (g s') in runState (g y) t')                            = {invertir let in}



Por otro lado,
((St f) >>= k) >>= h                                            = {def >>=}
(St (\s -> let (x,s') = f s
            in runState (k x) s')) >>= h
St (\s'' -> let (x',s''') = (\s -> let (x,s') = f s
                                    in runState (k x) s') s''
             in runState (h x') s''')                           = {aplicacion, renombramos s'' a s}
St (\s -> let (x',s''') = let (x,s') = f s
                            in runState (k x) s')
            in runState (h x') s''')


COMPLETAR
-}


-- b) Definir operaciones set :: s → State s () y get :: State s s que permiten actualizar el estado y leerlo, respectivamente.

set s = (\_ -> ((), s))

get = (\s -> (s,s))

{- Ejercicio 7:
El tipo de datos Cont r a representa continuaciones en las que dado el resultado de una función (de tipo a) y la continuación de la computación (a -> r), devuelve un valor en r. 
Probar que Cont r es una mónada. 
-}

data Cont r a = Cont ((a -> r) -> r)

runCont (Cont f) = f

instance Monad (Cont r) where
    return x        = Cont (\f -> f x)
    (Cont c) >>= g  = Cont (\f -> c (\a -> runCont (g a) f))


{-
Demostración de que se trata de una mónada
Para esto definimos runCont (Cont f) = f.
Entonces queda (Cont c) >>= g  = Cont (\f -> c (\a -> runCont (g a) f))

(monad.1) return x >>= g = g x
D/
return x >>= g                                      = {def return}
(Cont (\f -> f x)) >>= g                            = {def >>=}
Cont (\f -> (\f -> f x) (\a -> runCont (g a) f))    = {aplicacion}
Cont (\f -> (\a -> runCont (g a) f) x)              = {aplicacion}
Cont (\f -> runCont (g x) f)                        =
Cont (runCont (g x))                                =
g x

(monad.2) m >>= return = m
D/
(Cont c) >>= return                                 = {def >>=}
Cont (\f -> c (\a -> runCont (return a) f))         = {def return}
Cont (\f -> c (\a -> runCont (Cont (\g -> g a)) f)) = {def runCont}
Cont (\f -> c (\a -> (\g -> g a) f))                = {aplicacion}
Cont (\f -> c (\a -> f a))                          =
Cont (\f -> c f)                                    =
Cont c

(monad.3) m >>= (\x. k x >>= h) = (m >>= k) >>= h
D/
(Cont c) >>= (\x. k x >>= h)                                                    = {def >>=}
Cont (\f -> c (\a -> runCont ((\x. k x >>= h) a) f))                            = {aplicacion}
Cont (\f -> c (\a -> runCont (k a >>= h) f))                                    = {(1) k a = Cont c'}
Cont (\f -> c (\a -> runCont ((Cont c') >>= h) f))                              = {def >>=}
Cont (\f -> c (\a -> runCont (Cont (\f' -> c' (\b -> runCont (h b) f'))) f))    = {runCont}
Cont (\f -> c (\a -> (\f' -> c' (\b -> runCont (h b) f') f))                    = {aplicacion}
Cont (\f -> c (\a -> c' (\b -> runCont (h b) f))                                = {c' = runCont (k a)}
Cont (\f -> c (\a -> runCont (k a) (\b -> runCont (h b) f))                                = {c' = runCont (k a)}

-}
Cont (\f -> c (\a -> runCont (g a) f))


((Cont c) >>= k) >>= h                                                       = {def >>=}
(Cont (\f -> c (\a -> runCont (k a) f))) >>= h                               = {def >>=}
Cont (\g -> c (\f -> c (\a -> runCont (k a) f)) (\a -> runCont (h a) g))     = {aplicacion}
Cont (\g -> c (\a -> runCont (k a) (\a -> runCont (h a) g)))





{- Ejercicio 8:
Se desea implementar un evaluador para un lenguaje sencillo, cuyos términos serán representados por el tipo de datos T (definido debajo).
Se busca que el evaluador cuente la cantidad de divisiones, y reporte los errores de división por cero.
Se plantea el tipo de datos M (debajo) para representar una mónada de evaluación.
-}
data T = Con Int | Div T T

newtype M s e a = M {runM :: s → Either e (a, s)}

-- Entonces el evaluador puede escribirse de esta manera:
eval :: T -> M Int String Int
eval (Con n) = return n
eval (Div t1 t2) = do v1 ← eval t1
                      v2 ← eval t2
                      if v2 ≡ 0 then raise "Error: Division por cero."
                                else do modify (+1)
                                        return (v1 `div` v2)

-- y el cómputo resultante se podría ejecutar mediante una función auxiliar:
doEval :: T -> Either String (Int, Int)
doEval t = runM (eval t) 0

-- a) Dar la instancia de la mónada M s e.
instance Monad (M s e) where
    return x    = M (\s -> Right (x,s)) -- Right (x,s) = return_either (x,s)
    (M g) >>= f = M (\s -> case g s of
                            Left e = Left e
                            Right (x,s') = runM (g x) s')

-- b) Determinar el tipo de las funciones raise y modify, y dar su definición.


-- c) Reescribir eval sin usar notación do y luego expandir las definiciones de >>=, return, raise y modify, para obtener un evaluador no monádico.






{- Ejercicio 9:
Dado el tipo de datos M:
-}
data M m a = Mk (m (Maybe a))

-- a) Probar que para toda mónada m, M m es una mónada.
instance Monad (M m) where
    return x    = Mk (return (Just x))  -- Just = return_maybe
    (Mk mma) >>= f = Mk (do ma <- mma
                            case ma of
                                Nothing = return Nothing
                                Just a = f a)

COMPLETAR (demostración)

-- b) Definir una operación auxiliar throw :: Monad m ⇒ M m a que lanza una excepción.
throw = Mk (return Nothing)

-- c) Dada la mónada de estado StInt y el tipo N.
--    Definir operaciones get :: N Int y put :: Int → N (), que lean y actualizen (respectivamente) el estado de la mónada.
data StInt a = St (Int → (a, Int))
type N a = M StInt a

get :: N int
get = Mk (St (\n -> (Just n,n)))

put :: Int -> N ()
put n = Mk (St (\_ -> (Just (),n)))

-- d) Usando N, definir un intérprete monádico para un lenguaje de expresiones aritméticas y una sola variable dado por el siguiente AST.

data Expr = Var
            | Con Int
            | Let Expr Expr
            | Add Expr Expr
            | Div Expr Expr
{-
- El constructor Var corresponde a dereferenciar la única variable.
- Con corresponde a una constante entera,
- Let t, a asignar a la única variable el valor de la expresi´on t.
- Add y Div corresponden a la suma y la división respectivamente.
- La variable tiene un valor inicial 0.
- El intérprete debe ser una función total que devuelva el valor de la expresión y el valor de la (única) variable.
Por ejemplo, si llamamos a la única variable w, la expresión:
    let w = (2 + 3) in w /7, queda representada en el AST por la expresi+on:
Let (Add (Con 2) (Con 3)) (Div Var (Con 7)
-}
COMPLETAR
