module Checkers.Moves where

import Checkers.Types

-- Implement your code for moves function below
moves:: GameState -> (SMorJM [Move])
moves st
    | jumpmoves /= [] = JM (jumpmoves)
    | simplemoves /= [] = SM (simplemoves)
    | otherwise = EndM
    where
        jumpmoves = jump_moves st
        simplemoves = simple_moves st

simple_moves :: GameState -> [Move]
simple_moves st     -- Code borrowed from TA
                | status st == RedPlayer
                    = (simpleKing (redKings st) st)++(simplePiece (redPieces st) st)
                | status st == BlackPlayer
                    = (simpleKing (blackKings st) st)++ (simplePiece (blackPieces st) st)
                | otherwise = []

-- List comprehension
-- Start Position (x,y) from existing pieces (x,y) <- ps
-- End Position (x', y') is (x +/- 1, y + dir) dir = +1 if black, else -1
-- End Position should be on board  0 <= x' y' <= 7
-- Cannot move to occupied position
-- Man, I'm so lost in this tutorial
-- Whats in the if condition tho
simplePiece :: PieceState -> GameState -> [[PorK Coord]]
simplePiece xs st = [ [P (x, y), (if y' == 0 || y' == 7 then K else P) (x', y')] | (x, y) <- xs,
                        (x', y') <- [(x+1, y+ (dir st)), (x-1, y+(dir st))],
                        repeatPiece (x,y) (x',y') st,
                        notoccupied(x', y') st,
                        onboard (x', y')]

dir :: GameState -> Int
dir g
    | status g == RedPlayer = -1
    | otherwise = 1

onboard :: (Int, Int) -> Bool
onboard (x, y)
    | (x >= 0 && x <= 7 && y >= 0 && y <= 7) = True
    | otherwise = False

-- So it seems this one is currently busted
notoccupied :: (Int, Int) -> GameState -> Bool
notoccupied pos st
    | elem pos (redPieces st) = False
    | elem pos (blackPieces st) = False
    | elem pos (redKings st) = False
    | elem pos (blackKings st) = False
    | otherwise = True

-- Smth about repeated states
-- Why does life suck
--  1. Only check for repeated state in simpleKing
--  2. Store all simple king moves in history
--  3. Reset history = [] after simplePiece/jumpMoves (Cuz these moves are irreversable)
--  4. Find cycles in history

-- history = [(M,M')]
-- list of tuples, where first element is the initial position, and second is the position moved to
-- think:
--      history = [
--          (R1,R1'), (G1,G1'),
--          (R1',R1''), (G1',G1),
--          (R1'',R1'''), (G1,G1')
--          (R1''',R1), (G1',G1')
--      ]

--      Must track for different colours lols
--      movement RED = [] temporary list movement
--      movement BLACK = []
--      
--      looking at the red moves
--      [(R1''',R1)] -> (R1'',R1''')    First element of the tuple in list is the same as the last element of tuple applied, move changes
--      [(R1'',R1)] -> (R1',R1'')
--      [(R1',R1)] -> (R1,R1')          Elements cancel out
--      []                              Empty list means no movement was made
--      If both lists are empty after a king moves, then the state is repeated
-- Also can go backwards lol, so gotta change the checks or smth idk
simpleKing :: PieceState -> GameState -> [[PorK Coord]]
simpleKing ps st = [[K (x,y), K (x',y')] | (x,y) <- ps,
                        (x',y') <- [(x+1,y+1), (x-1,y+1), (x+1,y-1), (x-1,y-1)],    -- Change directions to be omni-directional
                        repeatKing (x,y) (x',y') st,
                        notoccupied(x', y') st,
                        onboard (x', y'),
                        not ((x', y') `elem` ((redPieces st) ++ (blackPieces st)))]

-- Jump_piece st ++ Jump_kings st
jump_moves:: GameState -> [Move]
jump_moves st
                | status st == RedPlayer
                    = (jumpKing (redKings st) st)++(jumpPiece (redPieces st) st)
                | status st == BlackPlayer
                    = (jumpKing (blackKings st) st)++ (jumpPiece (blackPieces st) st)
                | otherwise = []

-- Added a check to see if it's at the end of the board and is promoted
jumpPiece :: PieceState -> GameState -> [[PorK Coord]]
jumpPiece ps st = [ (if (promote y (status st)) then K else P) (x,y) : ys | (x,y) <- ps, ys <- jumpPiece' (x,y) [] (x,y) st]

jumpKing :: PieceState -> GameState -> [Move]
jumpKing xs st = [ K (x, y):ys | (x, y) <- xs, ys <- jumpKing' (x, y) [] (x, y) st] 

promote :: Int -> Status -> Bool
promote y st
    | y == 7 && st == BlackPlayer = True
    | y == 0 && st == RedPlayer = True
    | otherwise = False

-- start: Starting position
-- rem: opponent's pieces that were eaten
-- (x,y): current start position
-- (x',y'): Eaten piece on each jump
-- (x'', y''): Landed position
-- ys is the list of jump moves from recursively calling jumpking'
jumpKing' :: Coord -> [Coord] -> Coord -> GameState -> [[PorK Coord]]
-- 1. Jump in any direction
--      (x',y') = (x +/- 1, y +/- 1), (x'',y'') = (x +/- 2, y +/- 2)
-- 2. Jumping over an opponent's piece
-- 3. Landing position should be onboard
-- 4. Landing position should not be occupied, or (x'',y'') == start 
-- 5. Eaten piece should not be in rem (Do not eat already eaten piece)
jumpKing' start rem (x, y) st = 
    [K (x'', y''):ys | ((x', y'),(x'',y'')) <-  [((x+1,y+1),(x+2,y+2)),((x-1,y+1),(x-2,y+2)),((x+1,y-1),(x+2,y-2)),((x-1,y-1),(x-2,y-2))],
    repeatKing (x,y) (x'',y'') st,
    not ((x',y') `elem` rem) &&
    opponent_occupied(x', y') st &&
    (notoccupied (x'', y'') st || start == (x'',y'')) &&
    onboard(x'',y''),
    ys <- jumpOver(jumpKing' start ((x',y'):rem) (x'', y'') st)]
    -- For jump piece, we just need to check if we've reached the end of the board
    -- in this case, the piece becomes a king and we call jumpKing in place of jumpPiece

-- Pretty much just copied these from jumpKing'
-- Just gonna need to see what happens
-- This is going to need a promotion step, or else I'm donezo
jumpPiece' :: Coord -> [Coord] -> Coord -> GameState -> [[PorK Coord]]
jumpPiece' start rem (x,y) st = 
    [(if (promote y'' (status st)) then K else P) (x'',y''):ys |
    ((x',y'), (x'',y'')) <- [((x+1,y+(dir st)),(x+2,y+2*(dir st))),((x-1,y+(dir st)),(x-2,y+2*(dir st)))],  -- Somewhere here, gotta check the game state for who's turn it is I guess
    repeatPiece (x,y) (x'',y'') st,
    opponent_occupied (x',y') st,
    onboard (x'',y''),
    (notoccupied (x'',y'') st) || ((x'',y'') == start),
    ys <- jumpOver (if (promote y'' (status st)) then (jumpKing' start ((x',y'):rem) (x'',y'') st) else (jumpPiece' start ((x',y'):rem) (x'',y'') st)) ]

-- This is meant to check if the king move attempted is already in history lols
repeatKing :: (Int, Int) -> (Int, Int) -> GameState -> Bool
repeatKing (x,y) (x',y') st
    | elem [K (x',y'), K (x,y)] (history st) = False -- here I can check if they're aleady in the history list
    | otherwise = True

-- And this one's for regular pieces
-- I probably didn't need to do this lmao
repeatPiece :: (Int, Int) -> (Int, Int) -> GameState -> Bool
repeatPiece (x,y) (x',y') st
    | elem [P (x',y'), P (x,y)] (history st) = False -- here I can check if they're aleady in the history list
    | otherwise = True

jumpOver [] = [[]]
jumpOver z = z

opponent_occupied :: Coord -> GameState -> Bool
opponent_occupied xy st 
                | status st == RedPlayer
                    = xy `elem` blackPieces st || xy `elem` blackKings st
                | status st == BlackPlayer  
                    = xy `elem` redPieces st || xy `elem` redKings st
                | otherwise = False