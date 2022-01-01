module Ex2Types where

-- | Success or fail data type for Q3.
data SF a 
    = SS a
    | FF 
  deriving (Show, Eq, Ord, Read)

-- | 'collatz' function for Q3.
collatz :: Int -> Int
collatz n 
    | even n = n `quot` 2
    | otherwise = 3 * n + 1

-- | The 'machine epsilon' as given in the assignment for Q4.
e :: Double
e = 2 ^^ (-7)

-- | Matrix data types for Q6.
type Matrix a = [[a]]
type DoubleMatrix = Matrix Double

-- | Binary search tree data type for Q8.
data STree a
    = Node (STree a) a (STree a)
    | Leaf
  deriving (Show, Eq, Ord, Read)

-- | Rose tree data type for Q10.
data Rose a = RS a [Rose a]
  deriving (Show, Eq, Ord, Read)
