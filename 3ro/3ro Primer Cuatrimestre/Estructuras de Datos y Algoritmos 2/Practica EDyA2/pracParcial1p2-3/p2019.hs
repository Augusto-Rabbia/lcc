data Treap p k = E | N (Treap p k) p k (Treap p k)

key :: Treap p k -> k
key (N _ _ k _) = k
insertBST :: (Ord k, Ord p) => p -> k -> Treap p k -> Treap p k
insertBST p k E = (N E p k E)
insertBST p k (N t1 _ kt t2) = 
    if kt == k then
        (N t1 p kt t2)
    else if kt > k then
        insertBST p k t1
    else 
        insertBST p k t2


priority :: Treap p k -> p
priority (N _ p _ _) = p

isTreap :: (Ord k, Ord p) => Treap p k -> Bool
isTreap E = True
isTreap (N E _ _ E) = True
isTreap (N E p k (N l2 p2 k2 r2)) =
    if p2 <= p && k2 >= k then
        isTreap (N l2 p2 k2 r2) 
    else
        False

isTreap (N (N l1 p1 k1 r1) p k E) = 
    if p1 <= p && k1 <= k then
        isTreap (N l1 p1 k1 r1)  
    else
        False

isTreap (N (N l1 p1 k1 r1) p k (N l2 p2 k2 r2)) =
    if p1 <= p && p2 <= p && k1 <= k && k2 >= k then
        (isTreap (N l1 p1 k1 r1)) && (isTreap (N l2 p2 k2 r2))  
    else
        False

insert :: (Ord k, Ord p) => p -> k -> Treap p k -> Treap p k
insert p k E = (N E p k E)
insert p k (N t1 pt kt t2) =
    let (N l p' k' r) = if kt == k then
            (N t1 p kt t2)
        else if kt > k then
            insert p k t1
        else 
            insert p k t2
    in 
        rotarTreap (N l p' k' r)

rotateL (N l' p' k' (N l p k r)) = N (N l' p' k' l) p k r
rotateR (N (N l p k r) p' k' r') = N l p k (N r p' k' r')

rotarTreap (N E p k E) = (N E p k E)
rotarTreap (N E p k r) = if (priority r) > p then (rotateR (N E p k r))
    else (N E p k r)
rotarTreap (N l p k E) = if (priority l) > p then (rotateL (N l p k E))
    else (N l p k E)
rotarTreap (N l p k r) = if (priority l) > p then (rotateL (N l p k r))
    else if (priority r) > p then (rotateR (N l p k r))
        else (N l p k r)

split :: (Ord k, Ord p, Num p) => k -> Treap p k -> (Treap p k, Treap p k)
split x t = 
    let (N l p k r) = insert ((priority t) + 1) x t
    in (l, r)
