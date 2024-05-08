module ArrSeq where

import Test.HUnit
import Seq
import Par ((|||))
import qualified Arr as A


instance Seq A.Arr where

  emptyS = A.empty
  
  singletonS x = A.fromList [x]

  fromList xs = A.fromList xs
  
  lengthS arr = A.length arr
  
  nthS arr n = arr A.! n

  mapS f arr = tabulateS fun (lengthS arr)
                    where fun x = (f (nthS arr x))
  
  tabulateS f n = A.tabulate f n

  takeS arr n = A.subArray 0 n arr
  dropS arr n = A.subArray n ((lengthS arr) - n) arr

  filterS f arr | n == 0 = emptyS
                | n == 1 = if (f x) then (singletonS x) else emptyS
                | otherwise = let m = (div n 2)
                                  (a,b) = (filterS f (takeS arr m)) ||| (filterS f (dropS arr m))
                              in appendS a b
                    where n = (lengthS arr)
                          x = (nthS arr 0)

  appendS arr1 arr2 = tabulateS fun (n+(lengthS arr2))
                    where n = lengthS arr1
                          fun x | x >= n = (nthS arr2 (x-n))
                                | otherwise = (nthS arr1 x)

  showtS arr | len == 0 = EMPTY
             | len == 1 = ELT (nthS arr 0)
             | otherwise =
                    let mid = len `div` 2
                    in NODE (takeS arr mid) (dropS arr mid)
                where len = lengthS arr

  showlS arr | (lengthS arr) == 0 = NIL
             | otherwise = CONS (nthS arr 0) (dropS arr 1)
  
  
  joinS arrs = A.flatten arrs
  
  reduceS f b arr | len == 0 = b
                  | len == 1 = f b (nthS arr 0)
                  | otherwise = reduceS f b (contract f arr)
                  where len = lengthS arr
    
  scanS f b arr | len == 0 = (emptyS, b)
                | len == 1 = (singletonS b, f b (nthS arr 0))
                | otherwise = let 
                                contracted = contract f arr
                                (l,r) = scanS f b contracted
                                expanded = expand f arr l
                              in (expanded, r)
                    where len = (lengthS arr)

contract f arr | len == 0 = emptyS
               | len == 1 = arr
               | otherwise = let fun n = contractAux f arr n
                             in if (even len) then tabulateS fun (div len 2) 
                                else appendS (tabulateS fun (div len 2)) (singletonS (nthS arr (len - 1)))
                    where len = (lengthS arr)
  
contractAux f arr n = f (nthS arr m) (nthS arr (m+1))
                    where m = 2 * n
  

expand f arr results | len == 0 = emptyS
                     | len == 1 = arr
                     | otherwise = let fun x = expandAux f arr results x 
                                   in tabulateS fun len  
                        where len = (lengthS arr)

expandAux f arr res n = if (even n) then (nthS res (div n 2))
                                    else f (nthS res (div n 2)) (nthS arr (n-1))
