module ListSeq where

import Seq

data ListSeq a = ListSeq [a]

instance Seq [] where
  emptyS = []
  
  singletonS x = [x]
  
  lengthS xs = length xs
  
  nthS xs n = xs !! n

  mapS f xs = map f xs
  
  tabulateS f n = [f i | i <- [0..n-1]]
    
  filterS p xs = filter p xs
  
  appendS xs ys = xs ++ ys
  
  takeS xs n = take n xs
  
  dropS xs n = drop n xs
  
  showtS xs | null xs = EMPTY
            | length xs == 1 = ELT (head xs)
            | otherwise =
              let mid = (length xs) `div` 2
                  left = takeS xs mid
                  right = dropS xs mid
              in NODE left right
  
  showlS xs | null xs = NIL
            | otherwise =
              let x = head xs
                  xs' = tail xs
              in CONS x xs'
  
  joinS xss = concatMap (\xs -> xs) xss
  
  -- No eficiente (length y acc con aux)
  reduceS f acc xs | n == 1 = nthS xs 0
                   | otherwise = f (reduceS f acc (takeS xs (n `div` 2))) (reduceS f acc (dropS xs (n `div` 2)))
                      where n = (lengthS xs)

  {-scanS f acc xs =
    let (result, final) = reduceS (\(res, prev) x -> ((res ++ [f prev x]), f prev x)) ([acc], acc) xs
    in (result, final)
-}
  
  scanS f acc (z:zs) =
    let reduced = reduceS (\x y -> f x y) acc (z:zs)
    in (mapS (fst reduced), snd reduced)
  
  
  
  fromList xs = xs

{-
data Tree a = Leaf a | Node (Tree a) (Tree a)

  toTree :: Seq a -> Tree a 
  toTree s = case length s of 
            1 -> (Leaf nthS 0 s) 
            n -> Node (toTree(takeS s pp)) 
                      (toTree(dropS s pp)) 
            where pp= 2 ↑ ilg (n - 1)

  scanS f acc (ListSeq xs) =
    let (ListSeq result, final) = foldl (\(ListSeq res, prev) x -> (ListSeq (res ++ [f prev x]), f prev x)) (ListSeq [acc], acc) xs
    in (ListSeq result, final)
  
  fromList xs = ListSeq xs

-}


{-
  lengthS (ListSeq []) = 0
  lengthS (ListSeq (x:xs)) = 1+(lengthS (ListSeq xs))

  nthS 0 (ListSeq (x:xs)) = x
  nthS n (ListSeq (x:xs)) = nthS (n-1) (ListSeq xs)

  mapS f (ListSeq []) = (ListSeq [])
  mapS f (ListSeq (x:xs)) = (ListSeq ((f x):(mapS f (ListSeq xs))))

  filterS f (ListSeq []) = (ListSeq [])
  filterS f (ListSeq (x:xs))
-}