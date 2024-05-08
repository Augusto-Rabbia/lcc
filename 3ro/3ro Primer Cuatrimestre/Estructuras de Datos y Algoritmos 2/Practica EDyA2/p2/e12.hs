--XD NO ANDA

type NumBin = [Bool]

sumBin :: NumBin -> NumBin -> NumBin
sumBin [] [] = []
sumBin xs [] = xs
sumBin [] ys = ys

sumBin (x:xs) (y:ys) =
    let (suma, acarreo) = sumBits x y
        resto = sumBin xs ys
    in if acarreo then (suma:resto) else (suma:acarreo:resto)

sumBits :: Bool -> Bool -> (Bool, Bool)
sumBits False False = (False, False)
sumBits False True = (True, False)
sumBits True False = (True, False)
sumBits True True = (True, True)

