/*
 * John Abo
 * 30088517
 * CPSC 355 Project 1 part 1
 */

#include "bomberman.h"
//Included header file to allow for better readability
//regarding function documentation. All function documentation
//is done on the header file.


int main(int arg, char* argv[]) {
	
	FILE* score;
	
	//Important board information
	int width, length;
	int i = 0;
	char* scores[size]; //Stores the first 100 scores from the file
	int index[size]; //Represents the index of the scores array, will be used for sorting later
	int arraySize;
	for (i; i < size; i++) {//Populates index array
		index[i] = i;
		scores[i] = malloc(64 * sizeof(char));
	}
	
	//Player information
	char* playerName;
	double playerScore = 0;
	int lives = 3;
	int bombs = 3;
	int level = 1;
	int range = 1;
	
	//Seed the random number generated
	srand(time(0));
	
	//Checks the command line if the user's inputs are valid
	checkCmdArg(arg, argv, &width, &length);

	double tableHidden[width][length]; //Creates hidden array, used to hold scores and level data
	int tableShown[width][length]; //Creates table that determines what is shown to the player
	playerName = argv[1]; //Gets the players name from command line
	
	//Prompts users for highscores
	arraySize = copyScores(scores) - 1;
	sort(index, scores, arraySize);
	promptHighscore(index, scores, arraySize);

	//Creates new table, this is the first table
	initialize((double*)tableHidden, (int*)tableShown, width, length);
	
	//For debugging purposes, this line shows the hidden board
	displayHidden((double*)tableHidden, width, length);
	
	//Plays the game until the player has no lives or no bombs
	while (!winCondition((double*)tableHidden, (int*)tableShown, width, length) && loseCondition(lives, bombs)) {
		
		displayShown((double*)tableHidden, (int*)tableShown, width, length);
		printf("\nPlayer name: %s\n", playerName);
		printf("SCORE: %lf\n", playerScore);
		printf("LIVES: %d\n", lives); printf("BOMBS: %d\n", bombs);
		printf("LEVEL: %d\n", level);
		
		//This is where the player is prompted for their move
		playerMove((double*)tableHidden, (int*)tableShown, width, length, &range, &lives, &bombs, &playerScore);
		level++;
	}
	
	//Shows board one more time before game end and final score
	displayShown((double*)tableHidden, (int*)tableShown, width, length);
	printf("FINAL SCORE: %lf \n", playerScore);
	if ((score = fopen("highscore.log", "a")) == NULL) {
		fclose(score);
		farewell();
	}
	
	//Logs player's score to file
	fprintf(score, "%5.2lf %.5s %3d\r\n", playerScore, playerName, level);
	fclose(score);
	
	//Prompts user if they want to see highscores, their own will be included
	arraySize = copyScores(scores) - 1;
	sort(index, scores, arraySize);
	promptHighscore(index, scores, arraySize);
	
	//Frees all allocated memory
	for (i = 0; i < size; i++)
		free(scores[i]);
	
	return 0;  
}

int copyScores(char* score[size]) {
	
	FILE* in;
	int i, check;
	
	//Attempts to open file for reading
	if ((in = fopen("highscore.log", "r")) == NULL) {
		fclose(in);
		farewell();
	}
	
	//Used to hold file data, will be used to store scores into array
	char* reading = malloc(64 * sizeof(char));
	
	//Places scores into array
	for (i = 0; check != -1 && i < size; i++) {
		check = myFget(reading, 64, in);
		strcpy(score[i], reading);
	}
	
	free(reading);
	fclose(in);
	
	return i;
}

int myFget(char* s, int ArraySize, FILE* stream) {
	int i = 0;
	
	for (i; i < ArraySize; i++) {
		s[i] = fgetc(stream);
		
		if (s[i] == '\n') {
			s[i] = '\n';
			return i;
		}
		if (s[i] == -1 || s[i] == 255) {
			s[i] = '\n';
			return -1;
		}
	}
	s[i - 1] = '\n';
	
	return i;
}

int loseCondition(int lives, int bombs) {
	//Checks if the player is out of lives, returns 0 if true
	if (lives == 0) {
		printf("Out of lives!\n");
		printf("\n\tGAME OVER\n");
		return 0;
	}
	
	//Checks if the player is out of bombs, returns 0 if true
	if (bombs == 0) {
		printf("Out of bombs!\n");
		printf("\n\tGAME OVER\n");
		return 0;
	}
	
	return 1;
}

int winCondition(double* tableHidden, int* tableShown, int x, int y) {
	int i, j;
	
	//Runs through both hidden and shown array
	//If the shown array says the exit is shown, returns 1
	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			if (*((tableHidden + i*x) + j) == EXIT && (*((tableShown + i*x) + j) == 0)) {
				printf("\nExit found\n");
				printf("\n\tWIN\n");
				return 1;
			}
		}
	}
	
	return 0;
}

float randomNum(int max, int min, int neg) {
	float r; //Random number that will be returned
	
	//Ensures the max is at least greater  than the min, then generates and returns a random number
	if (max > min) {
		r = rand() & 0xFFFF; //Keeps numbers generated between 0 and 65535
		r /= 100; //allows for decimals to be included in the generation
		
		r = r - ((double)((int)r/(max - min)) * (max - min)); //Manipulates the number so that it is between min and max
		
		//Either returns the number as a float, depending on parameter sent.
		if (neg)
			return -r;
		else
			return r;
	}
	
	return 0;
}

void promptHighscore(int* index, char* scores[size], int arraySize) {
	FILE* in;
	int i;
	int n = 0, count = 0;
	char* printed;
	
	//Attempts to open file for reading
	if ((in = fopen("highscore.log", "r")) == NULL) {
		fclose(in);
		farewell();
	}
	
	printf("How many top scores would you like to see?\n");
	printf("(Enter 0 if you do not want to see): ");
	count = scanf("%d", &n);
	
	if (count == 0) {
		printf("\nInvalid input \n");
		farewell();
	}
	
	for (i = 0; i < n && i < arraySize; i++) {
		printf("Place %d: %s ", i + 1, scores[i]);
	}
}

void sort(int* index, char* score[size], int arraySize) {
	
	//This uses code similar to my implementation
	//of a selection sort from a previous project
	
	int i, indexOfHighest;
	char** bin = malloc(64 * sizeof(char));
	double numDouble;
	
	for  (i = 0; i < arraySize - 1; i++) {
		
		//Temporary values that will be used to swap values
		int tempIn, j = 0;
		char* tempSc = malloc(64 * sizeof(char));
		double high = 0;
		indexOfHighest = -1;
		
		//Looks for highest value and sets it to highest while recording the index
		for (j = i; j < arraySize; j++) {
			
			numDouble = strtod(score[j], bin);
			
			if (high < numDouble) {
				high = numDouble;
				indexOfHighest = j;
			}
		}
		
		//Swaps the values of the current index position and the highest values
		//found down the array
		
		if (index >= 0) {
			stpcpy(tempSc, score[i]); tempIn = index[i];		
			strcpy(score[i], score[indexOfHighest]); index[i] = index[indexOfHighest];
			strcpy(score[indexOfHighest], tempSc); index[indexOfHighest] = tempIn;
		}
		
		free(tempSc);
	}
	
	free(bin);
}

void playerMove(double* tableHidden, int* tableShown, int x, int y, int* range, int* lives, int* bombs, double* score) {

	int playerX, playerY;
	int i, j;
	int hitX, hitY;
	int currentRange = *range;
	int count;
	
	//Prompts user for coordinates
	printf("\nEnter any non-integer to end game");
	printf("\nEnter bomb position (x,y): ");
	count = scanf("%d %d", &playerX, &playerY);
	
	if (count < 1) {
		printf("\nInvalid input \n");
		farewell();
	}
	if (playerX >= x || playerX < 0) { //Returns to main function to reprompt player
		printf("\nPlease enter a valid x coordinate\n");
		return;
	}
	if (playerY >= y || playerY < 0) { //Returns to main function to reprompt player
		printf("\nPlease enter a valid y coordinate\n");
		return;
	}
	
	//reduces bomb count by 1
	(*bombs)--;
	(*range) = 1;
	printf("\nBoom\n"); //Some filler
	
	//Reveals the parts of the board where the player has placed their bomb
	for (i = playerY; i < playerY + ((2 * currentRange) + 1); i++) {
		for (j = playerX; j < playerX + ((2 * currentRange) + 1); j++) {
			
			hitY = i - currentRange;
			hitX = j - currentRange;
			
			//Ensures only hidden tiles are being revealed
			//Already shown files will not be taken into account
			if ((hitX >= 0 && hitX < x) && (hitY >= 0 && hitY < y)) {
				//Ensures the position being checked is withing the board
				//Then adds the score found in the cell
				
				if (*((tableShown + hitY * x) + hitX) == 1) {
					
					*((tableShown + hitY * x) + hitX) = 0;
					*score += *((tableHidden + hitY * x) + hitX);
					
					//If the space is a special tile, increase bomb count by 1
					if ((*range) < 10 && !(*((tableHidden + hitY * x) + hitX))) {
						printf("Range doubled! ");
						(*range) *= 2;
					}
					
					if (*((tableHidden + hitY * x) + hitX) == BONUS) {
						printf("More bombs! ");
						(*bombs)++;
					}
				}
			}
		}
	}
	
	//If the player gets a negative score, reduce their lives by 1 and return score to 0
	if ((*score) <= 0) {
		
		printf("\nreached negative score\n");
		
		(*lives)--;
		*score = 0;
	}
}

void initialize(double* tableHidden, int* tableShown, int x, int y) {
	
	int i,j, randInt;
	int negative = 0, total = 0;
	double fraction;
	double value;
	
	printf("\n\tCreating random Board... \n");
	
	//Generates table based on size given
	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			
			*((tableShown + i * x) + j) = 1;
			
			randInt = randomNum(100, 1, 0);
			
			//Ensures the value has a magnitude of 15
			//value = randInt & 0xF;
			
			//If the number returned is between 100 and 60
			//Which would make up 40% of the possible numbers generated
			//Then the value assigned will be a positive
			if (randInt > 60) {
				*((tableHidden + i * x) + j) = randomNum(15, 0, 0);
			} else if (randInt > 20) { //same idea as previous condition, another 40%
				negative++;
				*((tableHidden + i * x) + j) = randomNum(15, 0, 1);
			} else if (randInt > 10){ //Remaining 20 posibilities are for special spaces
				*((tableHidden + i * x) + j) = 0;
			} else {
				*((tableHidden + i * x) + j) = BONUS;
			}
			
			total++;
			
		}
	}
	
	fraction = negative * 100.0 / total;
	
	printf("\nThere are %lf percent negatives\n", fraction);
	
	//Places exit on board
	i = (int)randomNum(x, 0, 0);
	j = (int)randomNum(y, 0, 0);
	*((tableHidden + i * y) + j) = EXIT;
}

void displayHidden(double *tableHidden, int width, int length) {
	int i,j;
	
	printf("\nDisplaying table:\n");
	
	//Prints the hidden table for debugging uses
	for (i = 0; i < length; i++) {
		for (j = 0; j < width; j++) {
				printf("%-6.2lf ", *((tableHidden + i*width) + j));

		}
		printf("\n");
	}
}

void displayShown(double *tableHidden, int* tableShown, int width, int length) {
	int i,j;
	
	printf("\nDisplaying table:\n");
	
	//Runs through both tables, if the shown table gives 1, only show X
	for (i = 0; i < length; i++) {
		for (j = 0; j < width; j++) {
			
			if (*((tableShown + i * width) + j)) {
				printf("X ");
			} else {
				
				if (*((tableHidden + i * width) + j)== EXIT) { //Checks if the space being shown is the exit
					printf("* ");
				} else if (*((tableHidden + i * width) + j) == BONUS){
					printf("& ");
				} else if (*((tableHidden + i * width) + j) > 0) { //Checks if the space being shown is positive
					printf("+ ");
				} else if (*((tableHidden + i * width) + j) < 0) { //Checks if the space being shown is negative
					printf("- ");
				} else {
					printf("$ ");
				}
			}

		}
		printf("\n");
	}
}

void checkCmdArg(int a, char* argv[], int *x, int *y) {
	
	int argX;
	int argY;
	
	//If the number of arguments is not 4, the user did not input the right number of arguments
	if (a != 4) {
		printf("Your arguments were invalid\nPlease enter valid arguments");
		farewell();
	}
	
	argX = atoi(argv[2]);
	argY = atoi(argv[3]);
		
	if (argX < 10 || argY < 10) { //Ensures the size of the board is at least 10x10
		printf("Please enter a valid size");
		farewell();
	}
	
	//Gives size of board to variables in main
	*x = argX;
	*y = argY;
}

void farewell() {
	printf("\n\tFare well\n"); 
	exit(1);
}