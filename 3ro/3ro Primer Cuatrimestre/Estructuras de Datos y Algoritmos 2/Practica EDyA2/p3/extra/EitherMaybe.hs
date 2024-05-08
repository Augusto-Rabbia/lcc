data Either a b = Left a | Right b
data Maybe a = Nothing | Just a
isNothing :: Maybe a -> Bool
isNothing Nothing = True
isNothing _ = False
isJust :: Maybe a -> Bool

safehead :: [a] → Either String a
safehead [] = Left "head the lista vac´ıa!"
safehead xs = Right (head xs)