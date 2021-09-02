//Game

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *mainThread(void *param);
void *playerThread(void *id);
int randomNum(int max, int min);

char rowCompletion(char id);
char colCompletion(char id);
char upperDiagCompletion(char id);
char lowerDiagCompletion(char id);

void endCond();
void printBoard();
void failed(char* id);

//Struct that contains board and game data
typedef struct {
	char board[9];
	int turn;
	int endGame;
	char winner;
} shared;

//Struct that contains player data
typedef struct {
	char name;
	int threadID;
} player;

//Shared struct is global
shared play;

int main(int argc, char* argv[]) {
	
	//Initializes variables and player data
	int i, j;
	player pOne;
	player pTwo;
	
	pOne.name = 'X';
	pOne.threadID = 1;
	
	pTwo.name = 'O';
	pTwo.threadID = 2;
	
	pthread_t threads[3];
	
	//Initializes board
	play.turn = 0;
	play.endGame = 0;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			*((play.board + i * 3) + j) = '-';
		}
	}
	
	//Creating threads and verifying their creation
	if (pthread_create(&threads[0], NULL, mainThread, "main"))
		failed("main");
		
	if (pthread_create(&threads[1], NULL, playerThread, &pOne))
		failed("1");
		
	if (pthread_create(&threads[2], NULL, playerThread, &pTwo))
		failed("2");
	
	while (!play.endGame);
	
	//Ends all threads
	for (i = 0; i < 3; i++) {
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}

/*
 * Function ran by the main thread. Checks for winner as the game progresses,
 * and prints the board for the user. Then it will declare the results of the
 * at the end, if either a player thread has one or the board is full.
 */
void *mainThread(void *param) {
	
	char* name = param;
	play.winner = '-';
	
	while (!play.endGame) {
		
		//Waits for its turn before executing its block of code.
		while ((play.turn % 3 != 0) );
		
		printf("%s thread running\n", name);
		
		endCond();
		
		printBoard();
		
		play.turn++;
	}
	
	//Prints the winner or if it's a draw
	if (play.winner == '-')
		printf("draw\n");
	else
		printf("%c wins \n", play.winner);
	
	pthread_exit(0);
}

/*
 * Function run by player threads. Runs the check as well, to ensure that the
 * player does not continue to attempt to move even after the game is over.
 * The moves are chosen at random and ensured that it is only in an empty cell.
 */
void *playerThread(void *id) {
	
	player* tID = id;
	
	while (!play.endGame) {
		
		//Waits for its turn before executing it's block of code.
		while ((play.turn % 3 != tID -> threadID && !play.endGame) );
		srand(time(0));
		
		//Checks if the game is already over.
		endCond();
		
		//Generates random numbers until the position generated is valid
		int i = randomNum(0,3);
		int j = randomNum(0,3);
		
		while (*((play.board + i * 3) + j) != '-' && !play.endGame) {
			i = randomNum(0,3);
			j = randomNum(0,3);
		}
		
		//Places the mark only if the game isn't over
		if (!play.endGame)
			*((play.board + i * 3) + j) = tID -> name;
		
		play.turn++;
	}
	
	pthread_exit(0);
}

/*
 * Random number generater recycled from past projects. Allows user to pass min
 * and max values that improves usablility of rand().
 */
int randomNum(int min, int max) {
	int randomNumber;
	
	//Ensures the max is at least greater  than the min, then generates and returns a random number
	if (max > min) {
		randomNumber = (rand() % (max - min)) + min;
		return randomNumber;
	}
	
	return 0;
}

/*
 * Calls all the end condition functions to check if the game is over, and
 * ends the game when necessary.
 */
void endCond() {
	
	int i, j;
	
	//The if statements are meant to prevent them all being called once a
	//winner has been found.
	if (!play.endGame)
		play.winner = rowCompletion('X');
	if (!play.endGame)
		play.winner = rowCompletion('O');
	if (!play.endGame)
		play.winner = colCompletion('X');
	if (!play.endGame)
		play.winner = colCompletion('O');
	if (!play.endGame)
		play.winner = upperDiagCompletion('X');
	if (!play.endGame)
		play.winner = lowerDiagCompletion('O');
	
	
	//Checks if the board is full
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if(*((play.board + i * 3) + j) == '-')
				return;
		}
	}
	
	play.endGame = -1;
}

/*
 * Checks all the rows for the ID passed by the user, then returns
 * either the passed id if there is a set of the same ids or '-'
 * otherwise. The game is ended when a set is found
 */
char rowCompletion(char id) {
	int i;
	
	//Checks all three rows if they are all equal to the player's id
	//If all are the same, then it ends the game and returns the id
	for (i = 0; i < 3; i++) {
		if (*((play.board + i * 3) + 0) == id) {
			if (*((play.board + i * 3) + 1) == id) {
				if (*((play.board + i * 3) + 2) == id) {
					play.endGame = -1;
					return id;
				}
			}
		}
	}
	
	return '-';
}

/*
 * Checks all the columns for the ID passed by the user, then returns
 * either the passed id if there is a set of the same ids or '-'
 * otherwise. The game is ended when a set is found
 */
char colCompletion(char id) {
	int i;
	
	//Checks all three columns if they are all equal to the player's id
	//If all are the same, then it ends the game and returns the id
	for (i = 0; i < 3; i++) {
		if (*((play.board + 0 * 3) + i) == id) {
			if (*((play.board + 1 * 3) + i) == id) {
				if (*((play.board + 2 * 3) + i) == id) {
					play.endGame = -1;
					return id;
				}
			}
		}
	}
	
	return '-';
}

/*
 * Checks the diagonal from top left to bottom right for a set of the id passed
 * then returns id if there is a set, other wise it will return '-'
 */
char upperDiagCompletion(char id) {
	
	//Checks the upper diagonal with hard coded positions
	if (*(play.board) == id) {
		if (*(play.board + 4) == id) {
			if (*(play.board + 8) == id) {
				play.endGame = -1;
				return id;
			}
		}
	}
	
	return '-';
}

/*
 * Checks the diagonal from top right to bottom left for a set of the id passed
 * then returns id if there is a set, other wise it will return '-'
 */
char lowerDiagCompletion(char id) {
	
	//Checks the lower diagonal with hard coded positions
	if (*(play.board + 2) == id) {
		if (*(play.board + 4) == id) {
			if (*(play.board + 6) == id) {
				play.endGame = -1;
				return id;
			}
		}
	}
	
	return '-';
}

/*
 * Prints the board to visualize the game
 */
void printBoard() {
	int i,j;
	
	//Prints the board
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf(" %c ", *((play.board + i * 3) + j));
		}
		printf("\n");
	}
}

 /*
  * Prints a message stating it failed to create a thread.
  * It is almost impossible for this to happen according to the prof
  * but I put in the case that it does happen. Better safe than sorry.
  */
void failed(char* id) {
	printf("Failed to create thread %s \n", id);
}
