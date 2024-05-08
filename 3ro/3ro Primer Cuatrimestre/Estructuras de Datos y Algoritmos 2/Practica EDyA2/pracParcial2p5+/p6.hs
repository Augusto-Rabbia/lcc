data BTree e a = Empty | Node Int (BTree a) a (BTree a)

size Empty = 0
size (Node sz _ _ _) = sz
{-W_size(n) = a               ∈ O(1)
S_size(n) ∈ O(1)
-}
nth :: BTree a -> Int -> a
nth (Node _ l x r) n | n == sz = x
                     | n < sz = (nth l)
                     | otherwise = (nth r)
                       where sz = (size l) +1
{-W_nth ∈ O(h) con h la altura del árbol
D/
CB: W_nth(1) = a <= c*1 sii c >= a
PI: (HI) existe c en R/ W_nth(h) <= ch
W_nth(h) = W_nth(h-1)+W_size(h-1)+b = W_nth(h-1)+a+b = W_nth(h-1)+d <= {(HI)}
c(h-1)+d <= ch-c+d <= ch
si c >= d
Y vale si c = max(a,d). 
-}
cons :: a -> BTree a -> BTree a
cons x Empty = (Node 1 Empty x Empty)
cons x (Node sz l x r) = (Node sz+1 (cons x l) x r)
{- W_cons(h) ∈ O(h)
-}

tabulate :: (Int -> a) -> Int -> BTree a
tabulate f 1 = (Node 1 Empty (f 1) Empty)
tabulate f n = (Node n l (f half) r)
                where half = (div n 2)
                      (l, r) = (tabulateAux f 0 (half-1)) ||| (tabulateAux f (half+1) n)
{- W_tabulates(n) ∈ O(n) si f ∈ O(1)
W(tabulatesAux f 1) = W(f 1) + a
W(tabulatesAux f n) = 2*W(tabulates f n/2) + W(f n/2) + b

W(tabulatesAux f n) ∈ O(n) + O(∑ W(f n)) para f con costo arbitrario.
D/
PI: (HI) W(tabulatesAux f n m) <= c* ∑ W(f n)
W(tabualtesAux f 0 n) = 2*W(tabulates f n/2) + W(f n/2) + b  <= {(HI)}
c* 2(∑ W(f n/2)) + W(f n/2) + b                         <= {}

Nada no sé, medio complejo
-}


tabulateAux f lower upper | lower == upper = (Node 1 Empty (f lower) Empty)
                          | otherwise = (Node (upper-lower) l (f half) )
                            where half = (div (lower+upper) 2)
                                  (l, r) = (tabulateAux f lower (half-1)) ||| (tabulateAux f (half+1) upper)

map :: (a -> b) -> BTree a -> BTree b
map f Empty = Empty
map f (Node n l x r) = (Node n newL (f x) newR)
                        where (newL,newR) = (map f l) ||| (map f r)
{-
W(map f Empty) = a
W(map f (Node n x l r)) = W(map f l) + W(map f r) + W(f x) + b   ∈ O(∑ (f y))
                                                                   y ∈ (N n x l r)


-}

take :: Int -> BTree a -> BTree a


drop :: Int -> BTree a -> BTree a



