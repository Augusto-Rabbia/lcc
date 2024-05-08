module ListSeq where

import Par
import Seq

instance Seq [] where
  emptyS = []
  
  singletonS x = [x]
  
  lengthS xs = length xs
  
  nthS xs n = xs !! n

  mapS f [] = []
  mapS f (x:xs) = (z:zs)
                    where (z,zs) = (f x) ||| (mapS f xs)

  filterS f [] = []
  filterS f (x:xs) = if z then (x:zs) else zs
                      where (z,zs) = (f x) ||| (filterS f xs)

  tabulateS f 0 = singletonS (f 0)
  tabulateS f n = zs ++ z
                    where (z,zs) = (singletonS (f n)) ||| (tabulateS f (n-1))
  
  appendS xs ys = xs ++ ys
  
  takeS xs n = take n xs
  
  dropS xs n = drop n xs
  
  showtS [] = EMPTY
  showtS [x] = (ELT x)
  showtS (x:xs) = let mid = len `div` 2
                      (left,right) = (takeS xs mid) ||| (dropS xs mid)
                  in (NODE left right)
                    where len = length xs
  
  showlS [] = NIL
  showlS (x:xs) = (CONS x xs)
  
  joinS [] = []
  joinS (xs:xss) = xs ++ (joinS xss)
  
  reduceS _ b [] = b
  reduceS f b [x] = f b x
  reduceS f b xs = reduceS f b (contract f xs)
  
  scanS f b [] = ([], b)
  scanS f b [x] = ([b], f b x)
  scanS f b (x:xs) = let contracted = (contract f (x:xs))
                         (l,r) = (scanS f b contracted)
                         result = (expand f (x:xs) l)
                     in (result, r)
  
  fromList xs = xs

expand f _ [] = []
expand f [x] [y] = [y]
expand f (x:y:xs) (z:zs) = (z):a:b
                            where (a,b) = (f z x) ||| (expand f xs zs) 

contract f [] = []
contract f [x] = [x]
contract f (x:y:xs) = (z:zs)
                        where (z,zs) = (f x y) ||| (contract f xs)
