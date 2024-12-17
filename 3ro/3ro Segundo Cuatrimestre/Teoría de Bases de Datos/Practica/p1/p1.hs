import Parsing
import Data.Char
import Control.Monad
import Control.Applicative hiding (many)

-- E2

expr :: Parser Int
expr = do t <- term
          (do char '+'
              e <- expr
              return (t+e)
           <|> (do char '-'
                   e <- expr
                   return (t-e)
                <|> return t))

term :: Parser Int
term = do f <- factor
          (do char '*'
              t <- term
              return (f*t)
           <|> (do char '/'
                   t <- term
                   return (f `div` t)
                <|> return f))

factor :: Parser Int
factor = (int <|> (do char '('
                      e <- expr
                      char ')'
                      return e))

eval :: String -> Int
eval s = fst (head (parse (acceptParent expr) s))

-- E3

acceptParent :: Parser a -> Parser a
acceptParent p = do char '('
                    x <- acceptParent p
                    char ')'
                    return x
                 <|> p

-- E4
data Expr = Num Int | BinOp Op Expr Expr deriving Show
data Op = Add | Mul | Min | Div deriving Show

expr' :: Parser Expr
expr' = do t <- term'
           (do char '+'
               e <- expr'
               return (BinOp Add t e)
            <|> (do char '-'
                    e <- expr'
                    return (BinOp Min t e)
                 <|> return t))

term' :: Parser Expr
term' = do f <- factor'
           (do char '*'
               t <- term'
               return (BinOp Mul f t)
            <|> (do char '/'
                    t <- term'
                    return (BinOp Div f t)
                 <|> return f))

factor' :: Parser Expr
factor' = do i <- int
             return (Num i)
          <|> (do char '('
                  e <- expr'
                  char ')'
                  return e)


eval' :: String -> Expr
eval' xs = fst (head (parse expr' xs))

-- E5

data Basetype = DInt | DChar | DFloat deriving Show
type Hasktype = [Basetype]

{-
Primero, creemos una sintaxis concreta:

hask ::= base '->' hask | base
base ::= 'Int' | 'Char' | 'Float'

hask puede ser factorizado:
hask :: base ('->' hask | eps)

No se si es correcta, por lo del "many"
-}
hask :: Parser Hasktype
hask = do x <- base
          xs <- many (do string " -> "
                         base)
          return (x:xs)

base :: Parser Basetype
base = (do string "Int"
           return DInt) 
       <|>
       ((do string "Char"
            return DChar)  
       <|> 
       ((do string "Float"
            return DFloat) 
       <|>
       failure))

evalHask :: String -> Hasktype
evalHask xs = fst (head (parse hask xs))

-- E6
{-
Parser de listas:
list ::= '[' elems ']'
elems ::= val ", " elems | val
val ::= num | "'" char "'"
num ::= digit num | digit
char ::= a | b | ... | z
-}

type ValorIntChar = Either Int Char

list :: Parser [ValorIntChar]
list = (do char '['
           x <- (character <|> num)
           xs <- many (do string ", "
                          (character <|> num))
           char ']'
           return (x:xs))

character :: Parser ValorIntChar
character = (do char '\''
                c <- item
                char '\''
                return (Right c)
            ) <|> failure

num :: Parser ValorIntChar
num = (do n <- int
          return (Left n)
      ) <|> failure

evalIntCharL :: String -> [ValorIntChar]
evalIntCharL str = fst (head (parse list str))

-- E7
data Hasktype = DInt | DChar | DFloat | Fun Hasktype Hasktype deriving Show

termHask :: Parser Hasktype
termHask = (do char '('
               x <- term
               string ") -> "
               y <- term
               return (Fun x y))
            <|>
            (do char '('
                x <- term
                char ')'
                return x)
            <|>
            ((do x <- func
                 return x)
              <|> (do x <- type2
                      return x))

type2 :: Parser Hasktype
type2 = (do string "Int"
            return DInt) 
        <|> 
        ((do string "Char"
             return DChar)  
        <|> 
        ((do string "Float"
             return DFloat)))

func :: Parser Hasktype
func = (do char '('
           x <- type2
           char ')'
           y <- term
           return (Fun x y))
       <|> ((do x <- type2
                string " -> "
                y <- term
                return (Fun x y)))


-- E8

{-
Solucionar el problema de la recursión a la izquierda en el siguiente parser
expr :: Parser Int
expr = (do e <- expr
           (do char '+'
               t <- term
               return (e+t)
            <|> (do char '-'
                    t <- term
                    return (e+t)
                 <|> term))
       )

Si tenemos la regla:
A -> Aa | b
esta es equivalente a
A -> bA'
A' -> eps | aA'

Reemplazamos expr y term
expr -> term expr'
expr' -> eps | (’+’ term | ’-’ term) expr'
term -> factor term'
term' -> eps | (’*’ factor | ’/’ factor) term'

y estos quedan igual:
factor -> digit | '(' expr ')'
digit -> '0' | '1' | ... | '9'
digit := int
-}

expr8 :: Parser Int
expr8 = (do t <- term
            f <- expr'
            return (f t)
        )

-- Debe devolver con su signo
expr8' :: Parser (Int -> Int)
expr8' = (do char '+'
             t <- term
             f <- expr'
             return (\x -> (f (x+t)))
         ) <|>
         (do char '-'
             t <- term
             f <- expr'
             return (\x -> (f (x-t)))
         ) <|> return id

term8 :: Parser Int
term8 = (do e <- factor
            f <- term'
            return (f e)
        )

term8' :: Parser (Int -> Int)
term8' = (do char '*'
             t <- factor
             f <- term'
             return (\x -> (f (x*t)))
         ) <|>
         (do char '/'
             t <- factor
             f <- term'
             return (\x -> (f (div x t)))
         ) <|> return id


factor8 :: Parser Int
factor8 = (do char '('
             t <- expr
             char ')'
             return t
          ) <|> int

eval8 :: String -> Int
eval8 s = fst (head (parse expr8 s))

-- E9

-- COMPLETAR