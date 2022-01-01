module Checkers.ApplyMove where

import Checkers.Moves
import Checkers.Types

----------------------------------
----------------------------------
--                              --
--  APPLYING A MOVE to a state  --
--                              --
----------------------------------
----------------------------------

-- Some notes from tutorial
-- How in the hell am I supposed to apply the move AND add stuff to history??????
-- 

-- Everytime a king is moved, add that move to history[] and movement[] list
-- Correction, always add moves to history, but only check for repeat when a jump_king is done
-- bruh

-- TA's CODE:

apply_move :: Move -> GameState -> GameState
apply_move mv st = case (moves st) of
        EndM -> st{message = "!!GAME OVER!!"}
        SM simple -> if mv `elem` simple
            then make_simple_move mv st
            else st{message = "Bruh Simple"}
        JM jump -> if mv `elem` jump
            then make_jump_move mv st
            else st{message = "Bruh Jump"}
        
-- Doesn't technically do anything yet, just some skeleton that will actually
-- Looks like this is done tho
make_simple_move :: Move -> GameState -> GameState
make_simple_move [start,end] st
    | status st == RedPlayer && member start (redKings st)
        = st{redKings = replace start end (redKings st)             -- Change the position of the red king moved
            , status = change_player st
            , message = "Moved Red King"
            , history = [start,end]:(history st)}                   -- Adds the move to history (Does this for all guards)
    | status st == RedPlayer && member start (redPieces st)         -- Red pieced moved
        = st{redKings = replace start end (redKings st)
            , status = change_player st
            , message = "Moved Red Piece"
            , history = [start,end]:(history st)}
    | status st == BlackPlayer && member start (blackKings st)      --black king moved
        = st{blackKings = replace start end (blackKings st)
            , status = change_player st
            , message = "Moved Black King"
            , history = [start,end]:(history st)}
    | status st == BlackPlayer && member start (blackPieces st)     -- Black piece moved
        = st{redKings = replace start end (redKings st)
            , status = change_player st
            , message = "Moved Black Piece"
            , history = [start,end]:(history st)}
    | otherwise = st {message = "Invalid"}

-- Gotta do this lmao
-- Just a copy paste of simple moves
-- Now I gotta add some other garbage to remove the pieces that were jumped over
-- Also may need to recursively call some stuff
-- Okie dokie, if I can get one change to work, I'm officially in the clear
-- Tho it's like 9 PM, rip lmao
make_jump_move :: Move -> GameState -> GameState
make_jump_move (start:end) st
    | status st == RedPlayer && member start (redKings st)
        = st{redKings = replace start (getLast end) (redKings st)   -- Runs replace with the last element of jumps list (This is done for all guards)
            , blackPieces = remove start end (blackPieces st)       -- removes the pieces that were jumped over
            , blackKings = remove start end (blackKings st)         -- (These were done for the other guards as well)
            , status = change_player st
            , message = "Jumped Red King"
            , history = (start:end):(history st)}
    | status st == RedPlayer && member start (redPieces st)
        = st{redKings = replace start (getLast end) (redKings st)
            , blackPieces = remove start end (blackPieces st)
            , blackKings = remove start end (blackKings st)
            , status = change_player st
            , message = "Jumped Red Piece"
            , history = (start:end):(history st)}
    | status st == BlackPlayer && member start (blackKings st)
        = st{blackKings = replace start (getLast end) (blackKings st)
            , redPieces = remove start end (redPieces st)
            , redKings = remove start end (redKings st)
            , status = change_player st
            , message = "Jumped Black King"
            , history = (start:end):(history st)}
    | status st == BlackPlayer && member start (blackPieces st)
         = st{blackPieces = replace start (getLast end) (blackPieces st)
            , redPieces = remove start end (redPieces st)
            , redKings = remove start end (redKings st)
            , status = change_player st
            , message = "Jumped Black Piece"
            , history = (start:end):(history st)}
    | otherwise = st {message = "Invalid"}
    where
        -- Cheeky little get last function, since init is being weird
        getLast :: [PorK Coord] -> PorK Coord
        getLast (pk:pks)
            | pks == [] = pk
            | otherwise = getLast pks

-- Checks if the start is in the list of pieces given
member :: PorK Coord -> PieceState -> Bool
member (P (x,y)) st = elem (x,y) st
member (K (x,y)) st = elem (x,y) st

-- Moves the pieces
replace :: PorK Coord -> PorK Coord -> PieceState -> PieceState
-- Probably should just extract the coords from the p or k (I did that lol)
-- I think I'm going to need to check for a promotion somewhere
-- Maybe I'll split this for kings and pieces? I'm not sure how to go about this
-- Maybe I'll just take the L lol
replace st nx ps = [if check == getCoord st -- Checks if the element is where the piece is starting
                        then getCoord nx    -- Replaces it with the move that it made
                        else check |        -- Otherwise add to list normally
                        check <- ps]        -- Checks over all pieces

-- This does the removing from the jump
remove :: PorK Coord -> [PorK Coord] -> PieceState -> PieceState
remove st ns ps = [state | state <- ps, -- State is the original list, and I just need to check if it got jumped over. Somehow...
                    not (consumed st ns ps)]

-- Checks if a piece will be consumed in a jump
consumed :: PorK Coord -> [PorK Coord] -> PieceState -> Bool
consumed st [] ps = False
consumed st (n:ns) ps
    | elem (middle (getCoord st) (getCoord n)) ps = True -- If the middle is an element, then it was consumed
    | otherwise = consumed n ns ps -- This is the case where they are consumed
    where 
        middle :: Coord -> Coord -> Coord
        middle (x,y) (x',y') = (div (x' + x) 2, div (y' + y) 2) -- Splits the difference between the 2 coords to get the position between them
    -- Perhaps I could've folded this?? Or smth?? idk

-- Extracts the coord from a PorK coord, this helps with some comparisons
getCoord :: PorK Coord -> Coord
getCoord (K (x,y)) = (x,y)
getCoord (P (x,y)) = (x,y)

-- Change the player state
change_player :: GameState -> Status
change_player st
        | status st == RedPlayer = BlackPlayer
        | status st == BlackPlayer = RedPlayer
        | otherwise = GameOver  -- I forgot that his was think lmao