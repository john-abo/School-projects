-- | This module includes some types for the first Haskell assignment.
-- You are to NOT modify this file. Moreover, you are to NOT submit this file.
module Ex1Types (SF (..)) where

-- | This is the 'SF' type as defined in the assignment.
data SF a = SS a | FF -- converts units
  deriving (Show, Read, Eq, Ord)
