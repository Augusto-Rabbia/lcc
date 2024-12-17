-- Ej 1:

prod = \n:Nat. \m:Nat. n<Nat> (suma n) m 

modulo = \l:List Nat. sqrt (l<Nat> (x:Nat. xs: Nat. suma (prod x x) xs) 0)

-- Ej 2:
-- newtype MEstado m s a = Es {runEs :: s -> m (a,s)}
-- a
instance Monad MEstado where
    return a = Es (\s -> return_m (a,s))
    (Es f) >>= g = Es (\s -> let m = f s
                                 in m >>= (\(a,s) -> runEs (g a) s))

-- b
-- type Parser a = MEstado [] String a
falla = Es (\_ -> [])
item = Es (\s -> case s of
                    [] -> []
                    (x:xs) -> [(x,xs)])

-- c
p <+> q = Es (\s -> let r = p s
                        in if r == [] then q s else r)

-- Da la siguiente gramática para tipos:
-- Base := Int | Char | Bool
-- T := [Base] | Base -> T
-- Definir un tipo Type Haskell para representar las expresiones de tipo aceptadas por la gramática y un parser monádico 
perseTy :: Parser Type
-- que parsea las mismas. 

data Type = TList Base | TFun Base Type
data Base = TInt | TChar | TBool

parserTy = (do cadena "["
               b <- parserBase
               cadena "]"
               return (TList b)
            ) <+>
            (do b <- parserBase
                cadena "->"
                t <- parserTy
                return (TFun b t))

parserBase :: Parser Base
parserBase = (do cadena "Int"
                 return TInt
             ) <+>
             (do cadena "Char"
                 return TChar
             ) <+>
             (do cadena "Bool"
                 return TBool)
