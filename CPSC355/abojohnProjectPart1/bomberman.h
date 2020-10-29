#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EXIT 20
#define BONUS 25
#define size 100

void farewell();
/* Ends the program with an appropriate message for the user
 *
 * REQUIRES: NOTHING.
 * PROMISES: Ends the program with message.
 */
 
void displayHidden(double* tableHidden, int width, int length);
/* displayHidden prints out the hidden board that the user does not normally
 * get to see. This is mostly used for debugging and testing.
 *
 * REQUIRES: tableHidden to be populated, width and lenth > 0.
 * PROMISES: A table with dimensions width x length. All cells are spaced properly
 *           and display their score value.
 */
 
void displayShown(double* tableHidden, int* tableShown, int width, int length);
/* displayShown prints out the player's board with hidden and visible tiles.
 *
 * REQUIRES: tableHidden and tableShown to be populated, width and length > 0.
 * PROMISES: A table with dimensions width x length. All hidden cells are X and
 *           shown cells show either '+', '-', '&', '$' depending on value.
 */
 
void checkCmdArg(int argc, char* argv[], int* x, int* y);
/* checkCmdArg checks the command line arguments the player entered, and sets the
 * width and length that will be used for later use. If the user inputs invalid dimensions
 * then the program will tell the user an error message and end the program.
 * 
 * REQUIRES: argc and argv[] to contain values. If there are none, then the program will end.
 * PROMISES: The program does not end, width and length are set according to the player's input.
 */
 
void initialize(double* tableHidden, int* tableShown, int x, int y);
/* Populates both the hidden and shown board based on the user's input
 *
 * REQUIRES: tableHidden and tableShown points towards existing arrays
 * PROMISES: tableHidden contain points as well as special values for exit and bonuses,
 *           tableShown contains all 1 which means the cell is hidden.
 */
 
void playerMove(double* tableHidden, int* tableShown, int x, int y, int* range, int* lives, int* bombs, double* score);
/* Contains all the code that involves the player's move, such as getting player input for coordinates,
 * verifying said input, modifying tableShown depending on the coordinates set, adding scores together
 * based on the values in tableHidden, and lastly changing values for lives and bombs. Lives decrease if
 * the user's score drops to 0 or lower, and bombs increase every time a '&' call is found.
 *
 * REQUIRES: tableHidden, tableShown, range, lives, bombs, score all point to existing variables.
 *           tableHidden and tableShown contain appropriate values.
 * PROMISES: tableShown cells become 0 if they have been exposed by the player's bomb. range changes
 *           depending on how many special cells were uncovered. lives and bombs changed depending on
 *           the cells uncovered.
 */
 
void sort(int* index, char* score[size], int arraySize);
/* RECYCLED CODE FROM PAST PROJECT AND ASSIGNMENT
 * Using a selection sort algorithm, it will sort both index and score based on the double that exist
 * at the beginning of the string.
 *
 * REQUIRES: Each string in score start with a double, that allows for the program to record the double,
 *           index and score point to an existing array.
 * PROMISES: Index and score sorted from highest to lowest based on the double held by score's strings
 */
 
void promptHighscore(int* index, char* scores[size], int arraySize);
/* Prompts the user whether or not they want to see the top scores. Then prints a number of top scores
 * based on the user's input.
 * 
 * REQUIRES: index and scores point towards existing arrays, and arraySize is the number of lines that
 *           can be read from scores array.
 * PROMISES: Prints the top n scores based on the user's score, if n is greater than size of scores, will
 *           only print the existing scores.
 */
 
int copyScores(char* dub[size]);
/* Copies all the score from highscore.log file into an array, returns the number of lines read.
 *
 * REQUIRES: dub points to an array of strings, and each string is allocated memory.
 * PROMISES: The dub array contains all the lines read from the highscore file, returns the number
 *           of lines that contain text read from the file.
 */
 
int myFget(char* s, int ArraySize, FILE* stream);
/* RECYCLED CODE FROM PAST PROJECT AND ASSIGNMENT
 * Modified version of fgets that returns for useful information than the normal fgets function.
 * 
 * REQUIRES: s and stream point towards existing variables. ArraySize is the size of the string the program
 *           wants to read.
 * PROMISES: copies the string read into s, unless the string read was EOF, then returns EOF. Otherwise
 *           returns the index of the last character for the string copied.
 */
 
int loseCondition(int lives, int bombs);
/* Checks if the user has lost the game by checking if number of lives or bombs has reached 0.
 * The user is then given a message depending on which value was 0.
 * 
 * REQUIRES: NOTHING. Any integer value lives and bombs will allow this function to work
 * PROMISES: Returns 1 if the number of lives or bombs has reached 0, will then prints a message
 *           depending on the value that was < 0. Otherwise return 0.
 */
 
int winCondition(double* tableHidden, int* tableShown, int x, int y);
/* Checks if the user has won the game by seeing if the special exit is uncovered by checking if the
 * respective tableShown cell is set to 0.
 * 
 * REQUIRES: tableHidden and tableShown point to existing tables, x and y match the dimensions of both
 *           the tableHidden and tableShown arrays.
 * PROMISES: returns 1 once the player has met win conditions, otherwise returns 0.
 */
 
float randomNum(int m, int n, int negative);
/* Generates a random number between m and n, where m is the upper limit and n is the lower limit.
 * Negative determines the direction of the final return value.
 * 
 * REQUIRES: m be greater than n. If not, the function returns 0. neg be 0 if the program is to
 *           return a positve, otherwise it returns a positive.
 * PROMISES: Returns a random number between m and n
 */