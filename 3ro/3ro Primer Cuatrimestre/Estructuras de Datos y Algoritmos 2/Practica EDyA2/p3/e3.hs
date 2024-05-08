data CList a = EmptyCL | CUnit a | Cosnoc a (CList a) a

isEmptyCL :: CList a -> Bool
isEmptyCL EmptyCL = True
isEmptyCL _ = False

isCUnit :: CList a -> Bool
isCUnit (CUnit x) = True
isCUnit _ = False

headCL :: CList a -> Maybe a
headCL EmptyCL = Nothing
headCL (CUnit x) = Just x
headCL (Cosnoc x l y) = Just x

tailCL :: CList a -> Maybe (CList a)
tailCL EmptyCL = Nothing
tailCL (CUnit x) = Just EmptyCL
tailCL (Cosnoc x (CUnit z) y) = 
tailCL (Cosnoc x l y) = Just l

reverseCL :: CList a -> CList a
reverseCL EmptyCL = EmptyCL
reverseCL (CUnit x) = (CUnit x)
reverseCL (Cosnoc x l y) = (Cosnoc y (reverseCL l) x)

inits



