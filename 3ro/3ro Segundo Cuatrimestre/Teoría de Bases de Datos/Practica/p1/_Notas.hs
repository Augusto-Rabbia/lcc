Un parser es un program que analiza texto para determinar su estructura sintáctica.

Los parsers son una estructura que toma un String (texto), y devuelven un árbol de parseo *Tree*.
	- Pueden no requerir todo el texto, por lo que devuelven un árbol y el resto del texto no leido *(Tree, String)*
	- Una cadena puede parsearse de varias o ninguna manera, por lo que devuelve una lista *[(Tree, String)]
	- Finalmente, técnicamente un parser podría devolver algo que no sea un árbol *[(a, String)]*
Un parser se define entonces de la siguiente manera en Haskell:
	type Parser a = String -> [(a,String)]
(Nosotros sólo vamos a admitir parsers que devuelvan a lo sumo un elemento (o sea, una falla, lista vacía, o la única solución))


--------- Haskell

newtype Parser a = P (String -> [(a, String)])

--------------------------
-- Parser básico
--------------------------
item :: Parser Char
item = P (\e -> case e of
                    [] -> []
                    (x:xs) -> [(x,xs)])

failure :: Parser a -- falla siempre
failure = P (\_ -> [])

-- Devuelve su argumento sin consumir nada
return :: a -> Parser a
return x = x -> P (\e -> (x,e))

-- Aplicar un parser
parse :: Parser a -> String -> [(a, String)]
parse (P f) s = f s 

-------------------
-- Combinadores de parsers
------------------------
-- Elección de parser
(<|>) :: Parser a -> Parser a -> Parser a
p <|> q = P(\e -> case paeser p e of
                    [] -> parser q e
                    xs -> xs)

-- Secuenciación de parsers
p :: Parser (Char, Char)
p = do c <- item
       item
       d <- item
       return (c,d)

-------------------------
-- Primitivas derivadas
-- Estas no agregan expresividad pero sí simplifican el trabajo con parsers
------------------------
-- sat parsea un caracter que satisface un predicado
sat :: (Char -> Bool) 
sat p = do c <- item
           if p c then return c
                  else failure

-- Parsear un dígito y caracteres específicos:
digit :: Parser Char
digit = sat isDigit

char :: Char -> Parser Char
char c = sat (==c)

-- Parsear una cadena específica
string :: String -> Parser String
string [] = return []
string (x:xs) = do char x 
                   string xs
                   return (x:xs) -- esta linea puede estar mal

-- Aplicar un parser 0 o más veces
many :: Parser a -> Parser [a]
many p = many1 p <|> return []

-- Aplicar un parser 1 o más veces
many1 :: Parser a -> Parser [a]
many1 p = do x <- p
             xs <- many p
             return (x:xs)             

digit' :: Parser Int
digit' = do d <- digit
            return (read [d])


         
