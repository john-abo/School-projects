-- | Exercise 2 template.
-- Instructions. 
--      1. You are to write your solutions to the exercise in this file. You
--      may add extra functions in this file to implement your solution.
--
--      2. You are to comment your code indicating how you arrived at a
--      solution.
--
--      3. It is best to avoid Prelude and library functions not already
--      imported and develop your own code for your solutions.
--
--      4. Do NOT change the type or name of any of the template functions. 
--
--      5. Do NOT change the name of this file. 
--
--      6. Do NOT change the module name of this file.
--
--      7. To submit this to GradeScope, submit ONLY this file.
--
--      8. Have lots of fun :). Wahoo!!
--      "Fun"
--      There is no fun to be had.
--      Hell itself is a resort compared to the following lines of code

module Ex2 where 

-- This imports the required types and support functions for the questions.
import Ex2Types

-- This imports some standard Prelude functions for you to use.
import Prelude (Int, Float, Integer, Eq, Ord, Bool (..), Double, String, otherwise, Num, Integral, Fractional, abs, (+), (-), (^), subtract, (*), (/), signum, (==), (/=), (<), (<=), (>), (>=), compare, (||), (&&), not, rem, mod, div, quot, max, min, fromIntegral, toInteger, undefined, error, Show, show, Bounded, minBound, maxBound, seq)
import Debug.Trace

-- Append as given in lecture
append :: [a] -> [a] -> [a]
append [] ys = ys
append (x:xs) ys = x:(append xs ys)

-- Map as given in the lecture
map :: (a -> b) -> [a] -> [b]
map f as = [ f y | y <- as]

-- Concat, as given in the lecture using foldr
concat :: [[a]] -> [a]
concat [] = []
concat xss = myfoldr (append) [] xss

-- "Fold right" as given in lecture
-- Encodes an important partern of recursion (Primitive recursion)
myfoldr :: (a -> c -> c) -> c -> [a] -> c   -- what
myfoldr f k [] = k  -- [a] is the empty list
myfoldr f k (x:xs) = f x (myfoldr f k xs)

-- I'll probably need this later
-- The next two functions were given in the lecture
mapRose :: (a -> b) -> (Rose a) -> (Rose b)
mapRose f (RS a branches) = RS (f a) (map (mapRose f) branches)

foldRose :: (a -> [c] -> c) -> (Rose a) -> c
foldRose f (RS a branches) = f a (map (foldRose f) branches)

-- | Q1.
twoTautology :: ((Bool, Bool) -> Bool) -> Bool
twoTautology f = and [f (x,y) | x <- bools, y <- bools] -- Create list of all values f returns
                                                        -- If at any point there was a false, it
                                                        -- would return false because of the fold
    where
        and :: [Bool] -> Bool
        and xs = myfoldr (&&) True xs

        bools :: [Bool]
        bools = [True, False]

twoEquiv :: ((Bool, Bool) -> Bool) -> ((Bool, Bool) -> Bool) -> Bool
twoEquiv fa fb = and [(==) (fa (x,y)) (fb (x,y)) | x <- bools, y <- bools]  -- Passes the same x,y into the functions
                                                                            -- =='s the output, and foldr ANDs the resulting list
    where
        and :: [Bool] -> Bool
        and xs = myfoldr (&&) True xs

        bools :: [Bool]
        bools = [True, False]

-- | Q2
-- I gotta do this somehow. Or else no marks for me...
badFermat :: Integer
badFermat = listHead allFerm  -- listHead allFerm   -- Extracts head of list of non-prime values
                        -- Hardcoding the number while I try to figure out how to do this properly
                        -- Had this been true, then no number would have been generated
                        -- I think they want me to brute force it?? though I think they want a list comprehension type deal
                        -- smth like:
                        -- allFerm = , where ferm :: Integer -> Integer ferm n = (2^(2^n)) + 1
                        -- then I just need to fine one in the list that isn't prime
                        -- Expected 4294967297
        -- Generates list of all non-prime values
allFerm :: [Integer]
allFerm = [x | x <- [ ferm y | y <- [1..]], notPrime x]

ferm :: Integer -> Integer
ferm a = (2 ^ (2 ^ a)) + 1

-- Determines if integer passed is a prime
notPrime :: Integer -> Bool
notPrime a = myfoldr (||) False [(==) 0 (mod a x) | x <- [2..a - 1]] -- Checks if a is divisible by numbers from 1 to a

-- Gets head of list
listHead :: [a] -> a
listHead (x:xs) = x

-- | Q3
-- After watching some veritasium, I know if there's no index I'd pretty much be a legend in the math world
-- so that's not a case I need to worry about
collatzIndex :: Int -> SF [Int]
collatzIndex x  | x < 0 = FF                        -- This will only work for negatives
                | otherwise = SS (collatzHelp x)    -- Just make the resulting list a success
    where
        collatzHelp :: Int -> [Int]                 -- Effectively bypassing the Success Fail requirement of the original function
        collatzHelp 1 = [1]                         -- Base cases that will result in a loop upon reaching one
        collatzHelp 2 = [2,1]
        collatzHelp 4 = [4,2,1]
        collatzHelp x = x:(collatzHelp (collatz x)) -- Recursion

        collatz :: Int -> Int                       -- Finds the next value
        collatz x   | (mod x 2) == 1 = (3 * x) + 1  -- Odd number 3x+1
                    | otherwise = div x 2           -- Evn number x/2
-- | Q4
-- Gotta look up what exactly this wants me to do
-- Note several days later, I think it wants to find when a function is equal to zero???
-- Ok, having actually read it properly, this one doesn't seem so bad. I lied, I'm timing out
-- Man, even after looking it up and seeing how close my solution is to those online. Why doesn't this work??
bisection :: (Double -> Double) -> (Double, Double) -> SF Double
bisection f (a,b)   | (b - a) < e = FF    -- The width of [a,b] is smaller than the acceptable range
                                                -- I think this is the culprit
                    | closeEnough (f mid) = SS mid
                    -- These were passing the tests, but somehow the expected value doesn't match the actual
                    | ((*) (f a) (f mid)) < 0 = bisection f (a,mid)    -- First 2 cases where root is in [a,mid]
                    | ((*) (f b) (f mid)) < 0 = bisection f (mid,b)    -- Second 2 cases where root is in [mid,b]
                    -- Lets try this again
                    -- | True = undefined -- what do I even check for here lmao
                    | otherwise = FF                                        -- Last case, mid is the root of the function
    where
        mid :: Double
        mid = (b - a) / 2

        -- I made the mistake of using a instead of x earlier lol
        closeEnough :: Double -> Bool
        closeEnough x   | (abs x) < e = True
                        | otherwise = False 

-- | Q5
-- These are going to be interesting
-- This was given in lecture, just had to use the function given
bsort :: (a -> a -> Bool) -> [a] -> [a]
bsort f [] = []
bsort f (x:xs) = push (f) (x) (bsort f xs)
    where
        push g x [] = [x]
        push g x (y:ys) | (g x y) = x:(y:ys)
                        | otherwise = y:(push (g) x ys)

-- Bubblesort as given in the lecture
--mybs :: (Ord a) => [a] -> [a]
--mybs [] = []
--mybs (x:xs) = push x (mybs xs)
--    where
--        push x [] = [x]
--        push x (y:ys)   | x <= y = x:(y:ys)
--                        | otherwise = y:(push x ys) 

-- This was given in both lecture and tutorial
-- Lecture showed how to do it without the function
-- Tutorial showed me how to use the not function
qsort :: (a -> a -> Bool) -> [a] -> [a]
qsort f (x:xs) = append (append (qsort f l) [x]) (qsort f r)
    where
        l = [y | y <- xs, f y x]
        r = [y | y <- xs, not (f y x)]
qsort _ _  = []

-- Not sure how to go about splitting a Haskell list, but we'll see
-- This one is also going to suck
-- The only solution I had in mind was more akin to insertion sort...
-- no way in hell am I fixing this to be faster. I've given you part of my soul and you ask for my life
msort :: (a -> a -> Bool) -> [a] -> [a]
msort _ [] = []
msort f as = help f [[x] | x <- as] -- creates list of singletons of all elements. This is for bottom up sort
    where

        -- Takes list of lists, and merges it all and whatnot. whatever man, I'm so tired
        help :: (a -> a -> Bool) -> [[a]] -> [a]
        help _ [last] = last                -- Last singleton
        help f lst = help f (sublist f lst) -- Merges the singletons

        sublist :: (a -> a -> Bool) -> [[a]] -> [[a]]
        sublist f (a:b:ls) = (merge f a b):(sublist f ls)   -- Takes first 2 elements and merges
        sublist f ls = ls                                   -- If all that's left is a singleton, just return it

        -- Merges 2 lists
        merge :: (a -> a -> Bool) -> [a] -> [a] -> [a]
        merge _ xs [] = xs  -- ys is shorter than xs
        merge _ [] ys = ys  -- xs is shorter than ys
        merge f (x:xs) (y:ys)   | f x y     = x:(merge f xs (y:ys))   -- Put x at the beginning of the list 
                                | otherwise = y:(merge f (x:xs) ys)   -- Put y at the beginning of the list
-- | Q6
-- How do I do go about this with lists. I could probably use a fold for this
-- Fold each element by appending each to a list of lists? idk lol
-- Maybe even list comprehension
transpose :: Matrix a -> SF (Matrix a)
transpose [] = SS [[]]  -- Base case
-- The meat and potatoes of this problem. I think this wants list comprehension. OR maybe even mapping
transpose (m:ms) =  case transpose ms of
                        SS [[]] -> SS (makeCol m)   -- Creates a new column made from m
                        SS mat -> attach m mat      -- Add m to the beginning of all rows
                        FF -> FF                    -- Otherwise fail
    where
        attach :: [a] -> Matrix a-> SF (Matrix a)   -- Used to attach [a] as the first column in [[a]]
        attach [] ys = SS ys
        attach (x:xs) (y:ys) =  case attach xs ys of
                                    SS mat -> SS ((x:y):mat)    -- appends x to the beginning of y, appends that row to the matrix
                                    FF -> FF                    -- Otherwise fail lol
        attach _ _ = FF

        makeCol :: [a] -> [[a]]                     -- Creates column from given row of elements
        makeCol [] = []
        makeCol (x:xs) = [x]:(makeCol xs)

-- This one wasn't bad
addMat :: DoubleMatrix -> DoubleMatrix -> SF DoubleMatrix
addMat [] [] = SS [[]]
addMat (x:xs) (y:ys) =  case (addMat xs ys) of  -- Check if the next case is a fail or success
                            SS [[]] -> SS [(addRow x y)]    -- Last Row
                            SS xss -> SS ((addRow x y):xss) -- Add row and append to previous list
                            FF -> FF                        -- It was a fail
    where
        addRow :: [Double] -> [Double] -> [Double]          -- Just adding each element in a row
        addRow [] [] = []
        addRow (x:xs) (y:ys) = (x + y):(addRow xs ys)
addMat _ _ = FF
 

-- I gotta remember how to multiply matrices lols
-- This one's gonna suck bruh
-- Perhaps it requires a similar process to transpose
-- Imma spend all of tomorrow working on this
multMat :: DoubleMatrix -> DoubleMatrix -> SF DoubleMatrix
multMat [] [] = SS [[]]     -- Base Case
multMat (x:xs) m2 = case multMat xs m2 of
                        --SS [[]] -> --Man idek anymore
                        FF -> FF
multMat _ _ = FF

-- | Q7.
-- These were given in lecture, thank the lord
nreverse :: [a] -> [a]
nreverse [] = []
nreverse (x:xs) = append (nreverse xs) [x]  -- Appends the last element to the end of another reverse
                                            -- Append is expensive tho, and requires a traversal of
                                            -- the entire list

freverse :: [a] -> [a]
freverse as = help as []                    -- Helper function, a function with better parameters
    where
        help :: [a] -> [a] -> [a]
        help [] ys = ys                     -- If the list passed is empty, return the reversed list
        help (x:xs) ys = help xs (x:ys)     -- append to beginning of new reversed list

-- Except for this one, I'm gonna need to figure out what the identity function is
-- Oh wait, it just returns what was passed into it lol, ok sounds good
-- This was also given in the notes on the professor's page and during lectures
hreverse :: [a] -> [a]
hreverse as = myfoldr (\ a f -> (\x -> f(a:x))) id as []
    where   -- Identity function just returns what it was given
        id :: a -> a
        id x = x
-- Grade scope for some reason was passing this test even tho I haven't writtin
-- anything for this yet

-- | Q8.
-- May just take the L at this point
isAVL :: Ord a => STree a -> Bool
isAVL = undefined

-- | Q9.
-- Given in tutorial
-- But broken because map isn't included for this assignment
all_paths :: Rose a -> [[a]]
all_paths (RS a rs) = go a (map all_paths rs)   -- returns a list of all paths of subtrees [[[a]]]
    where 
        go a [] = [[a]]
        go a xs = map (a :) (concat xs)         -- Add a to the beginning of all lists in xs

-- | Q10.
-- 6 ways to compute factorials (I checked out the site and it was really amusing)
-- I like how it was computed with foldr and foldl

-- 1. Freshman Haskell Programmer (Pretty decent, but could've used guards or patterns in place of if else)
--      fac n = if n == 0
--                  then 1
--                  else n * fac (n-1)
-- 2. Sophomore Haskell Programmer (This person just did what 1. did, but with a lambda function?)
--      fac = (\(n) -> 
--              (if ((==) n 0)
--                  then 1
--                  else ((*) n fac ((-) n 1))))
-- 3. Junior Haskell Programmer (Using patterns and starting from the largest number and moving down)
--      fac 0 = 1
--      fact (n+1) = (n+1) * fac n  -- (where n + 1 = input, such than n = input - 1)
-- 4. Another Junior Haskell Programmer (Same as before, but instead of using n, it just passes n - 1)
--      fac 0 = 1
--      fac n = n * fac (n-1)
-- 5. Senior Haskell Programmer (Using foldr, which multiplies all elements of the list)
--      fac n = foldr (*) 1 [1 .. n]
-- 6. Senior Haskell Programmer (This time using foldl, to start folding at the beginning of the list)
--      fac n = foldl (*) 1 [1 .. n]
factorialOf1891 :: Integer
factorialOf1891 = fac 1891
    where
        fac :: Integer -> Integer       -- Just going to use senior haskell programmer's method since it makes the most sense to me   
        fac n = myfoldr (*) 1 [1 .. n]  -- Folding a list from 1 to n with the multiplication function

-- | Q11.
-- Optional? That means it's never getting done lol
widthRose :: Rose a -> Int
widthRose = undefined

-- Haskell you have taken everything from me.
-- Happiness is no longer a feeling I am familiar with.
-- The abyss beckons, and I answer the call,
-- riding on black mane and ebony scales
-- The dark demon Haskell shall ferry me to the void.