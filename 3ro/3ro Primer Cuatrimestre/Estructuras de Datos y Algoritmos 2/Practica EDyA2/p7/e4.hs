data Paren = Open | Close
data TreeView a = EMPTY | ELT a | NODE ([a]) ([a])

showT :: [a] -> TreeView a
showT xs  | null xs = EMPTY
          | length xs == 1 = ELT (head xs)
          | otherwise =
              let mid = div (length xs) 2
                  left = take mid xs
                  right = drop mid xs
              in NODE left right


matchP :: [(Int, Int)] -> (Int, Int)
matchP s = case showT s of
    EMPTY -> (0,0)
    ELT v -> v
    NODE l r -> let
                    l' = matchP l
                    r' = matchP r
                in combine l' r'

combine :: (Int, Int) -> (Int, Int) -> (Int, Int) 
combine (i, j) (i', j') = if j < i' then (i+(i'-j), j')
                                    else (i, j+j'-i')


transform :: Char -> (Int, Int)
transform '(' = (0,1)
transform ')' = (1,0)
