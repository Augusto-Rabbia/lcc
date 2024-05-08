tad List (A: Set) where
    import Bool
    nil     :: List A
    cons    :: A -> List A -> List A
    null    :: List A -> Bool 
    head    :: List A -> A
    tail    :: List A -> List A
    inL     :: List A -> A -> Bool
    delAll  :: List A -> A -> List A
null nil = True
null (cons x xs) = False
head (cons x xs) = x
tail (cons x xs) = xs
inL nil y = False
inL (cons x xs) y = if x == y then True else (inL xs y)
delAll nil y = nil
delAll (cons x xs) y = if x == y then (delAll xs y) else (cons x (delAll xs y))

-- especificación de Listas con secuencias como modelo
nil = {}
cons x {x1,...,xn} = {x,x1,...,xn}
head {x1,...,xn} = x1
tail {x1,x2,...,xn} = {x2,...,xn}

--  Ej 2)
tad Pila (A: Set) where
    import Bool
    empty   :: Pila A
    push    :: A -> Pila A -> Pila A
    isEmpty :: Pila A -> Bool 
    top     :: Pila A -> A
    pop     :: Pila A -> Pila A
-- Especificación algebráica
isEmpty empty = True
isEmpty _ = False
top (push x p) = x
pop (push x p) = p

--  Ej 3)
tad Conjunto (A : Set) where
    import Bool
    vacio        :: Conjunto A
    insertar     :: A → Conjunto A → Conjunto A
    borrar       :: A → Conjunto A → Conjunto A
    esVacio      :: Conjunto A → Bool
    union        :: Conjunto A → Conjunto A → Conjunto A
    interseccion :: Conjunto A → Conjunto A → Conjunto A
    resta        :: Conjunto A → Conjunto A → Conjunto A
-- Esp alg
borrar y (insertar x c) = if y == x then (borrar c) else (insertar x (borrar c)) 
esVacio vacio = True
esVacio (insertar x c) = False
union vacio c = c
union (insertar x c1) c = (insertar x (union c1 c))
interseccion vacio c = vacio
interseccion (insertar x c1) (insertar y c2) = 
    if (x == y) then (insertar x (interseccion c1 c2)) else (interseccion c1 c2)
resta vacio c2 = vacio
resta c1 vacio = c1 
resta (insertar x c1) (insertar y c2) =
    if (x == y) then (resta c1 c2) else (insertar x  (resta c1 c2)) -- o (resta y c2)?

-- Ej 4) En este, es posible que no  se tenga la prioridad aparte, sino que la prioridad es parte del elemento.
-- En ese caso, por ejemplo, primero (poner x s) = max x (primero s), primero (poner x vacio) = x
tad PriQ (A : Set) (B : Ordered Set) where
    import Bool
    vacia   :: PriQ
    poner   :: A -> B -> Int -> PriQ -> PriQ 
    primero :: PriQ -> A
    sacar   :: A -> PriQ -> PriQ
    esVacia :: PriQ -> Bool
    union   :: PriQ -> PriQ -> PriQ
primero (poner x p vacia) = x
primero (poner x p (poner y p' q)) = 
    if p < p' then primero (poner y p' q) else primero (poner x p q)
sacar (poner x p vacia) = x
sacar (poner x p (poner y p' q)) =
    if p < p' then poner x p (sacar (poner y p' q)) else poner y p' (sacar poner x p q)
union vacia q2 = q2
union q1 vacia = q1
union (poner x p q1) (poner y p' q2) = 
    if x == y then if p < p' then (poner x p' (union q1 q2)) else (poner x p (union q1 q2))
        else (poner x p (poner y p' (union q1 q2)))

--  Ej 5)
tad BalT (A : Ordered Set) where
    import Maybe
    empty   : BalT A
    join    : BalT A → Maybe A → BalT A → BalT A
    size    : BalT A → N
    expose  : BalT A → Maybe (BalT A, A, BalT A)
size empty = 0
size (Join l Nothing r) = (size l) + (size r)
size (join l (Just x) r) = 1 + (size l) + (size r)

expose empty = Nothing
expose (Join l Nothing r) = expose newL x newR
    where l' = balTSearch l
          r' = 
          x = 
          newL =
          newR = 
??
expose (Join l (Just x) r) = (Just ((balTSearch l) x (balTSearch r)))

--  Ej 6)  (uncurry zip) ◦ unzip = id
Hagamoslo por inducción estructural sobre el tad de listas:
CB: l = []
(uncurry zip) (unzip [])    = {unzip.1}
(uncurry zip) ([],[])       = {uncurry zip.1}
[]
PI: (HI) (uncurry zip) . (unzip) ps
    l = ((x,y):ps)
(uncurry zip) (unzip ((x,y):ps))    = {unzip.2}
(uncurry zip) (x:xs,y:ys)               = {uncurry zip.2}
            where (xs,ys) = unzip ps
(x,y):(zip xs ys)                       = {uncurry}
    where (xs,ys) = unzip ps
(x,y):(uncurry zip (xs,ys))             = {where}
    where (xs,ys) = unzip ps
(x,y):(uncurry zip (unzip ps))          = (HI)
(x:y):(id ps)                           = {id}
(x:y):ps

--  Ej 7)
sum xs <= length xs * maxl xs
CB: l = [] 
sum [] = {sum.1} = 0 = 0*0 = {length.1, maxl.1} = length [] * maxl []

PI: (HI) sum xs <= length xs * maxl xs
l = (x:xs)
sum (x:xs)                                                  = {sum.2}
x + (sum xs)                                                <= (HI)
x + (length xs * maxl xs)                                   <= {def max}
(x `max` (maxl xs)) + (length xs * (x `max` (maxl xs)))     =
(1 + (length xs)) * (x `max` (maxl xs))                     = {length.2, maxl.2}
length (x:xs) * (maxl (x:xs))

--  Ej 8)
data Arbol a = Hoja a | Nodo a (Arbol a) (Arbol a)
size :: Arbol a -> Int
size Hoja a = 1
size (Nodo x l r) = 1 + (size l) + (size r)

-- b) Existe k e N/ size t = 2k+1.
-- D/ 
Dem por ind estructural sobre Arbol.
CB: t = Hoja a
size (Hoja a) ={size.1}= 1 = 1+2*0
Y existe k = 0
PI: (HI1) Existe h en N/ size l = 2h+1
    (HI2) Existe j en N/ size r = 2j+1
t = (Nodo x l r)
size (Nodo x l r)               = {size.2}
1 + (size l) + (size r)         = {(HI1), (HI2)}
1+2h+1+2j+1 = 2(h+j+1)+1 pimba

-- c-d)
mirror Hoja a = Hoja a
mirror (Nodo x l r) = (Nodo x (mirror r) (mirror l))
-- mirror . mirror = id. 
-- D/
Dem por ind estructural sobre el tad Arbol.
CB: t = Hoja a
mirror (mirror (Hoja a)) = Hoja a = id Hoja a
PI: (HI1) mirror . mirror l = id l = l
    (HI2) mirror . mirror r = id r = r
t = (Nodo x l r)
mirror . mirror (Nodo x l r)                        = {compo}
mirror(mirror (Nodo x l r))                         = {mirror.2}
mirror (Nodo x (mirror r) (mirror l))               = {mirror.2}
(Nodo x (mirror (mirror l)) (mirror (mirror r)))    = {(HI1), (HI2)}
(Nodo x l r) ={id}= id (Nodo x l r)

-- e)
Dem por i
CB: t = Hoja a
hojas (Hoja a) ={hojas.1}= 1 < 2 = 2^1 ={altura.1}= 2^(altura (Hoja a))
PI: (HI1) hojas l < 2^(altura l)
    (HI2) hojas r < 2^(altura r)
t = (Nodo x l r)
hojas (Nodo x l r)                                              = {hojas.2}
(hojas l) + (hojas r)                                           < {(HI1) (HI2)}
2^(altura l) + 2^(altura r)                                     < {max}
2^(max (altura l) (altura r)) + 2^(max (altura l) (altura r))   =
2*2^(max (altura l) (altura r))                                 =
2^(1+(altura l) `max` (altura r))                               = {altura.2}
2^(altura (N x l r))

--  Ej 10)
Dem por ind estr que Vt::Tree a map f . flatten t = flatten . mapTree f t, para cualquier f :: (a -> b)
CB: t = Leaf x
map f (flatten (Leaf x)) ={flatten.1}= map f [x] = [(f x)] 
flatten (mapTree f (Leaf x)) = flatten (Leaf (f x)) = [(f x)] 
PI: (HI1) map f (flatten l) = flatten (mapTree f l)
    (HI2) map f (flatten r) = flatten (mapTree f r)
t = (Node x l r)
map f (flatten (Node x l r))                                    = {flatten.2}
map f ((flatten l) ++ [x] ++ (flatten r))                       = {map}
(map f (flatten l)) ++ map f [x] ++ (map f (flatten r))         = {(HI1) (HI2)}
(flatten (mapTree f l)) ++ [(f x)] ++ (flatten (mapTree f l)) 


flatten (mapTree (Node x l r))                                  = {mapTree.2}
flatten (Node (f x) (mapTree f l) mapTree (f r))                = {flatten.2}
(flatten (mapTree f l)) ++ [(f x)] ++ (flatten (mapTree f l)) 

--  Ej 11)
Dem por ind estr sobre listas:
-- a)
CB: l = []
join (map singleton []) ={map}= join (singleton []) ={singleton}= join [[]] = join ([]:[]) ={join.2}= [] ++ join [] ={join.1}= []++[] = []
PI: (HI) join (map singleton xs) = id xs = xs
l = (x:xs)
join (map singleton (x:xs))             = {map}
join ((singleton x):(map singleton xs)) = {singleton, join.2}
[x] ++ (join (map singleton xs))        = {(HI)}
[x] ++ (id xs) = [x] ++ xs ={++}= (x:xs)
-- b)
CB: l = []
join (join []) ={join.1}= join [] ={join.1}= []
join (map join []) ={map}= join [] = []
PI: (HI) join (join xss) = join (map join xss)
l = (xss:xsss)
join (join (xss:xsss))                  = {join.2}
join (xss ++ (join xsss))               = {Lema}
(join xss) ++ (join (join xsss))        = {(HI)}
(join xss) ++ (join (map join xsss))    = {Lema}
join ((join xss) ++ (map join xsss))    = {map}
join (map join (xss:xsss))

Lema:
join (ll ++ yss) = (join ll) ++ (join yss)
CB: ll = []
join ([] ++ yss) ={++}= join yss ={++}= [] ++ (join yss) ={join.1}= (join []) ++ (join yss)
PI: (HI) join (zss ++ yss) = (join zss) ++ (join yss)
ll = (zs:zss)
join ((zs:zss) ++ yss)              = {join.2}
zs ++ (join (zss ++ yss))           = {(HI)}
zs ++ ((join zss) ++ (join yss))    = {asociatividad ++?}
(zs ++ (join zss)) ++ (join yss)    = {join.2}
(join (zs:zss)) ++ (join yss)
Y queda demostrado

--  Ej 12)
-- a)
insert :: Ord a => a -> Bin a -> Bin a
insert a Hoja = Nodo Hoja a Hoja
insert a (Nodo l b r) | a <= b    = Nodo (insert a l) b r
                      | otherwise = Nodo l b (insert a r)
Vt :: Bin tal que t es un BST, (insert x t) es un BST
    Por ind estructural sobre Bin
CB: t = Hoja 
insert x Hoja = (Nodo Hoja x Hoja)
Ahora, (Nodo Hoja x Hoja) es un BST, pues, Hoja son BSTs por definición,
y sean L = {c/ c es una clave en un nodo de Hoja}
       R = {c/ c es una clave en un nodo de Hoja}
Vemos que x >= c_L para todo c_L en L (pues c_L vacío),
y de la misma forma,  x <= c_R para todo c_R en R

PI: (HI1) (insertar x l) es un BST
    (HI2) (insertar x r) es un BST
t = (Nodo l e r)
(insertar x l e r) 