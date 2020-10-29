bomberman.h contains all function documentation, as well as defining constants.
This code has been tested on both school servers and my home computer, however the script
file was generated on my home computer. Any feature seen to work on my home computer
does indeed work on school servers, and this can be tested by compiling and running the
code.

When executing the game, enter:
	./<executablename> <playername> <width> <length>

<width> and <length> must be 10 or higher, and there is no maximum.
<Playername> should not contain a space and only the first 5 characters will be saved
for the highscore.

When prompted for coordinates, enter the 2 numbers with a space
between the 2 values. (Example: 1 1, 10 10, 12 9)

Game ends when player dies, and will continue to the next level once
player finds the exit represented by the '*' character.
There will be format errors when printing the top scores, but the list
will still be sorted and properly.

Only 100 scores will be recorded!

LEGEND:
	'+' - This cell increased your score
	'-' - This cell decreased your score
	'$' - Number of bombs increased
	'*' - This is the exit, you will move to the next level when this is found