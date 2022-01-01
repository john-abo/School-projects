module Checkers.ApplyMove where

import Checkers.Moves
import Checkers.Types

------------------------------------------------
------------------------------------------------
--
--  APPLYING A MOVE to a state
--
------------------------------------------------
------------------------------------------------

-- Everytime a king is moved, add that move to history[] and movement[] list
apply_move:: Move -> GameState -> GameState
apply_move mv st = st{message = "!!GAME OVER!!"}
