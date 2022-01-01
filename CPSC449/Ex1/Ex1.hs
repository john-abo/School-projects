-- God save our souls

-- | Exercise 1 template.
-- Instructions. 
--      1. Write your solutions to the exercise in this file. You
--      may add extra functions in this file to implement your solution.
--
--      2. Do NOT change the type or name of any of the template functions. 
--
--      3. Do NOT change the name of this file. 
--
--      4. Do NOT change the module name of this file.
--
--      5. To submit this to GradeScope, submit ONLY this file.
--
--      6. Have lots of fun :) -- No :(
module Ex1 where

-- This includes the required types for this assignment (namely, the 'SF' type).
import Ex1Types

-- This imports some standard library functions for you to use. 
import Prelude (Int, Float, Integer, Eq, Ord, Bool (..), String, otherwise, abs, (+), (-), subtract, (*), (/), (==), (/=), (<), (<=), (>), (>=), (||), (&&), rem, mod, div, quot, max, min, fromIntegral, undefined, error, show)

-- This includes helpful functions for debugging.
import Debug.Trace

-- Append as given in lecture
append :: [a] -> [a] -> [a]
append [] ys = ys
append (x:xs) ys = x:(append xs ys)

myfoldr :: (a -> c -> c) -> c -> [a] -> c   -- what
myfoldr f k [] = k  -- [a] is the empty list
myfoldr f k (x:xs) = f x (myfoldr f k xs)

notPrime :: Integer -> Bool
notPrime a = myfoldr (||) False [(==) 0 (mod a x) | x <- [2..a - 1]] -- Checks if a is divisible by numbers from 1 to a

testList :: Integer -> [Integer]
testList a = [(mod a x) | x <- [2..a - 1]]

-- | Q1. 
avgThree :: Int -> Int -> Int -> Float
avgThree x y z = (a + b + c) / 3
    where
        --Used these to convert Int inputs to a Float
        --From documentation, it seems fromIntegral
        a = fromIntegral x :: Float
        b = fromIntegral y :: Float
        c = fromIntegral z :: Float

-- | Q2. There's an existing max function
maxThree :: Int -> Int -> Int -> (Int, Int)
maxThree x y z  | x > y && x > z = (x,1)    -- x is only max
                | y > x && y > z = (y,1)    -- y is only max
                | z > x && z > y = (z,1)    -- z is only max
                | y > x && y == z = (y,2)   -- x is not max
                | x > y && x == z = (x,2)   -- y is not max
                | x > z && x == y = (x,2)   -- z is not max
                | x == y && x == z = (x,3)  -- All are max
                -- And that should cover all the cases
                -- This was shown in class, perhaps I can make it more efficient
                -- In terms of characters, that is

-- | Q3. 
-- check a b c, a is incremented by 1, b is multiplied by a, c is the input
-- SF: "Success/Fail" data type. Allows for cleaner error handling
-- I could probably do this using the case of thing

invFac :: Integer -> SF Integer
invFac n    | n == 0 = SS 1
            | n < 0 = FF
            | otherwise = check 1 1 n
        where
            check :: Integer -> Integer -> Integer -> SF Integer
            check a b c | ((a + 1) * (b * a)) > c = SS a            -- Once the *next* factorial is greater than the input, return current
                        | otherwise = check (a + 1) (b * a) (c)     -- Keep searching

-- | Q4. 
-- GCD(a,b) = GCD(b,r) where a = c*b + r as seen from math 271 notes
myGcd :: Int -> Int -> Int
myGcd q r   | r == 0 && q >= 0 = q          -- Once the remainder is 0, we can use GCD(a,0) = 0
            | r == 0 && q < 0 = (-q)        -- Quick and dirty fix, I'm not sure this is how it should work        
            | otherwise = myGcd r (mod q r) -- Euclid Algorithm

-- | Q5.
-- compute numerator and denominator recursively
binom :: Integer -> Integer -> Integer
binom n k   = div (numer n k) (denom k) --Compute the numerator and denominator seperately
            where
                numer n k   | k == 0 = 1
                            | otherwise = (n - (k - 1)) * (numer n (k - 1))
                denom k     | k == 0 = 1
                            | otherwise = k * (denom (k - 1))

-- | Q6.
-- My solution to this is effectively just creating my own function with nicer parameters
grow :: String -> String 
grow ls = growth 1 ls
    where
        -- grow function, but with better parameters, that allow me to pass the position of the character
        growth :: Int -> String -> String
        growth _ [] = []  -- terminates recursion
        growth i (s:ss) = append (expand i (s:[])) (growth (i + 1) ss) -- Appends expanded string with the rest of the expanded strings

        -- expands a given char
        expand :: Int -> String -> String
        expand x c  | x == 0 = []
                    | otherwise = append c (expand (x - 1) c)


-- | Q7. 
-- using n:ns, recusrively test n < (instrictorder ns)
-- Exit on [] = True, otherwise = False
instrictorder :: [Int] -> Bool
instrictorder [] = True                             -- Edge case where the input is the empty list
instrictorder (n:ns)    | ns == [] = True           -- If there is only 1 element in the list, should always be true
                        | otherwise = check n ns    -- Uses check helper function
                        where
                            -- check function recusively traverses the list until it reaches the end or is not in strict order
                            check :: Int -> [Int] -> Bool
                            check a (b:c)   | a > b = False
                                            | c == [] = True
                                            | otherwise = check b c

-- | Q8. 
-- Was pretty simple, based on the filter function given in lectures
-- Could probably do this better with list comprehension?
cheapItems :: [(String, Int)] -> Int -> [String]
cheapItems [] _ = []
cheapItems ((s, i):is) c    | i < c = s:(cheapItems is c)   -- Cost is cheaper
                            | otherwise = (cheapItems is c) -- Cost is not cheaper

-- | Q9. 
-- I can implement quicksort. It's just a bit messy
-- Based on list comprehension quicksort without the fancy operators included in prelude. Instead using homebrew functions given during lecture
sortByCost :: [(String, Int)] -> [(String, Int)]
sortByCost [] = []
sortByCost ((s,x):xs) = append (append (sortByCost (partLT xs x)) [(s,x)]) (sortByCost (partGT xs x)) -- Since I don't have the fancy ++ from prelude, I just manually appended
    where
        -- These functions are the partition functions for quicksort
        -- Pretty much code taken from cheapItems
        partGT :: [(String, Int)] -> Int -> [(String, Int)]
        partGT [] _ = []
        partGT ((s, i):is) c    | i >= c = (s,i):(partGT is c)
                                | otherwise = (partGT is c) 

        partLT :: [(String, Int)] -> Int -> [(String, Int)]
        partLT [] _ = []
        partLT ((s, i):is) c    | i < c = (s,i):(partLT is c)
                                | otherwise = (partLT is c)

-- | Q10. 
-- I think I'll need to define a set of all primes perhaps? Nvm, the tests have some massive prime
-- Maybe I'll do a next prime function, and see what happens from there
-- Prime factorization scares me, but it worked
-- I just need to clean the list of repeats, maybe I can use the 'case of' key word????
divisors :: Integer -> [Integer]
divisors x  | x <= 0 = []
            | otherwise = primef 2 x
    where
        primef :: Integer -> Integer -> [Integer]
        primef _ 0  = []    -- Can't divide by zero
        primef _ 1  = []    -- Once you reach one, it should just terminate to avoid dividing by 0
        primef p n  | (mod n p) == 0 = (repeating p (primef p (div n p)))   -- Similar idea to the Sieve of that greek dude
                    | otherwise = (primef (p + 1) n)            -- that we covered in class

        repeating :: Integer -> [Integer] -> [Integer]
        repeating a [] = (a:[])
        repeating a (x:xs)  | a == x = (x:xs)   -- Don't add a to the list
                            | otherwise = (append (a:[]) (x:xs))
-- | Q11. 
-- Pray the TA just does this one lul
-- Doesn't seem like they did, but I got an idea
-- It's around this point I wonder why the tests are taking so long
-- Also, I did this first try no bug checking. Feels good man
substring :: String -> String -> Bool
substring [] _ = True
substring needle haystack = subcheck needle needle haystack -- Names borrowed from documentation. effectively calling my own function with
                                                            -- better parameters again, like in grow
    where
        subcheck :: String -> String -> String -> Bool
        subcheck _ [] _ = True                              -- Once the needle string is empty, the check is over and the word is found
        subcheck _ _ [] = False                             -- Once the haystack is empty but the needle isn't found, return false
        subcheck original (x:xs) (y:ys) | x == y = (subcheck original xs ys)            -- Characters are the same pass check the next characters
                                        | otherwise = (subcheck original original ys)   -- Characters are not the same, back to original

-- | Q12. 
-- Using list comprehension, I can find the sublists containing x and those without x, with each recusive call it reaches the sublists
-- including all the elements.
-- Shout out to the Sept. 21 lecture for explaining list comprehension lol
sublists :: [a] -> [[a]]
sublists [] = [[]]            -- The empty list returns a list containing the empty
sublists (x:xs) = append (sublists xs) [x:sub | sub <- (sublists xs)]   -- First argument of append:
                                                                        --      All the sublists without head x
                                                                        -- Second argument of append:
                                                                        --      All the sublists WITH head x, using list comprehension
                                                                        -- this is the entire list of sublists

-- I've attained true enlightenment