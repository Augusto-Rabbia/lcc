data Exp = Lit Int | Add Exp Exp | Sub Exp Exp | Prod Exp Exp | Div Exp Exp

eval :: Exp -> Int
eval (Lit x) = x
eval (Add x y) = (+) (eval x) (eval y) 
eval (Sub x y) = (-) (eval x) (eval y) 
eval (Prod x y) = (*) (eval x) (eval y) 
eval (Div x y) = div (eval x) (eval y) 
