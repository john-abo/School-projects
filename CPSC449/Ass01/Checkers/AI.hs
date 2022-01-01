-- AI or whatever
-- moves gives a list of possible moves, that becomes branches on the game state tree
-- Each level alternates between red turn and black turn
-- Branches will be pruned with alpha beta pruning
-- :godisdead:

-- Heuristic shit again
-- compute a number to determine how good the game state is

-- red_heuristic = smth like # of Red - # of Black
-- opposite for black
-- Maybe add more weight for the kings
-- Want to maximize this heuristic (i.e more reds than blacks)
-- But we know the opponent wants to maximize theirs, so we 
-- choose the branches where the opponent's pieces are minimized