-- Fibonazi
fibSeq :: Int -> Seq Int
fibSeq 0 = fromList [1,0]
fibSeq 1 = fromList [1,0,1]
fibSeq n = appendS a (singletonS b)
        where (a, b) = scanS multMatFib (fromList [0,0,0,0]) (tabulateS (\_ -> (fromList [1,1,1,0]))))
-- Esto, pero sin hacerlo con listas pq S_fromList(n) e O(n)


multMatFib :: [Int] -> [Int]
multMatFib [a, b, c, d] = fromList [a+b, a, c+d, c]  

