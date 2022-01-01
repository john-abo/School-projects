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
simple_moves st
    | status st == RedPlayer = (simplePiece (redPieces st) st) ++ (simpleKing (redKings st) st)
    | status st == BlackPlayer = (simplePiece (blackPieces st) st) ++ (simpleKing (blackKings st) st)
    | otherwise = []

-- List comprehension
-- Start Position (x,y) from existing pieces (x,y) <- ps
-- End Position (x', y') is (x +/- 1, y + dir) dir = +1 if black, else -1
-- End Position should be on board  0 <= x' y' <= 7
-- Cannot move to occupied position
-- Man, I'm so lost in this tutorial
-- Whats in the if condition tho
simplePiece :: PieceState -> GameState -> [[PorK Coord]]
simplePiece ps st = [[P (x,y), (if (y' == 0 || y' == 7) then K else P) (x',y')] | (x,y) <- ps,
                        (x',y') <- [(x+1,y+dir st), (x-1,y+dir st)],
                        onBoard (x', y'),
                        not ((x', y') `elem` ((redPieces st) ++ (blackPieces st)))]

dir :: GameState -> Int
dir st
    | status st == RedPlayer = -1
    | status st == BlackPlayer = 1

onBoard :: Coord -> Bool
onBoard (x,y) = x >= 0 && x <= 7 && y >= 0 && y <= 7

notOccupied :: Coord -> GameState -> Bool
notOccupied (x,y) st =  not ((x,y) `elem` (redPieces st)) &&
                        not ((x,y) `elem` (redPieces st)) &&
                        not ((x,y) `elem` (redPieces st)) &&
                        not ((x,y) `elem` (redPieces st))

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
                        (x',y') <- [(x+1,y+dir st), (x-1,y+dir st)],    -- Change directions to be omni-directional
                        onBoard (x', y'),
                        not ((x', y') `elem` ((redPieces st) ++ (blackPieces st)))]

-- Jump_piece st ++ Jump_kings st
jump_moves :: GameState -> [Move]
jump_moves st
    | status st == RedPlayer = (jumpPiece (redPieces st) st) ++ (jumpKing (redKings st) st) 
    | status st == BlackPlayer = (jumpPiece (blackPieces st) st) ++ (jumpKing (blackKings st) st)

-- Added a check to see if it's at the end of the board and is promoted
jumpPiece :: PieceState -> GameState -> [[PorK Coord]]
jumpPiece ps st = [ (if (y == 0 || y == 7) then K else P) (x,y) : ys | (x,y) <- ps, ys <- jumpPiece' (x,y) [] (x,y) st]

jumpKing :: PieceState -> GameState -> [[PorK Coord]]
jumpKing ps st = [ K (x,y) : ys | (x,y) <- ps, ys <- jumpKing' (x,y) [] (x,y) st] 

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
jumpKing' start rem (x,y) st = 
    [K (x'',y''):ys |
    ((x',y'), (x'',y'')) <- [((x+1,y+1),(x+2,y+2)),((x+1,y-1),(x+2,y-2)),((x-1,y+1),(x-2,y+2)),((x-1,y-1),(x-2,y-2))],
    opponentOccupied (x',y') st,
    onBoard (x'',y''),
    (notOccupied (x'',y'') st) || ((x'',y'') == start),
    ys <- jumpOver (jumpKing' start ((x',y'):rem) (x'',y'') st)]
    -- For jump piece, we just need to check if we've reached the end of the board
    -- in this case, the piece becomes a king and we call jumpKing in place of jumpPiece

-- Pretty much just copied these from jumpKing'
-- Just gonna need to see what happens
jumpPiece' :: Coord -> [Coord] -> Coord -> GameState -> [[PorK Coord]]
jumpPiece' start rem (x,y) st = 
    [K (x'',y''):ys |
    ((x',y'), (x'',y'')) <- [((x+1,y+1),(x+2,y+2)),((x+1,y-1),(x+2,y-2)),((x-1,y+1),(x-2,y+2)),((x-1,y-1),(x-2,y-2))],  -- Somewhere here, gotta check the game state for who's turn it is I guess
    opponentOccupied (x',y') st,
    onBoard (x'',y''),
    (notOccupied (x'',y'') st) || ((x'',y'') == start),
    ys <- jumpOver (jumpPiece' start ((x',y'):rem) (x'',y'') st)]

jumpOver [] = [[]]
jumpOver z = z

opponentOccupied :: Coord -> GameState -> Bool
opponentOccupied (x,y) st
    | status st == RedPlayer = (x,y) `elem` (blackPieces st) || (x,y) `elem` (blackKings st)
    | status st == BlackPlayer = (x,y) `elem` (redPieces st) || (x,y) `elem` (redKings st)