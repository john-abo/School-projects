#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "images/defaultBack.h"
#include "images/challenge.h"
#include "images/frog.h"
#include "images/vehicle3.h"
#include "images/vehicle2.h"
#include "images/log3.h"
#include "images/log2.h"
#include "images/lives.h"
#include "images/score.h"
#include "images/moves.h"
#include "images/block72.h"
#include "images/block144.h"
#include "images/NUMBERS/zero.h"
#include "images/NUMBERS/one.h"
#include "images/NUMBERS/two.h"
#include "images/NUMBERS/three.h"
#include "images/NUMBERS/four.h"
#include "images/NUMBERS/five.h"
#include "images/NUMBERS/six.h"
#include "images/NUMBERS/seven.h"
#include "images/NUMBERS/eight.h"
#include "images/NUMBERS/nine.h"
#include "images/valuePack.h"
#include "images/timer.h"
#include "images/cursor.h"
#include "images/menu.h"
#include "images/winEnd.h"
#include "images/loseEnd.h"
#include <unistd.h>
#include <wiringPi.h>
#include "initGPIO.h"

//John Abo 30088517
//Hilton Luu 30085893

//Defines the pin nunber for CLK, DAT, and LAT
#define LAT 9
#define DAT 10
#define CLK 11
#define frogY picture -> f.yOffset
#define frogX picture -> f.xOffset
#define resetRow1 frogY = 144; livesCount -= 1;
#define resetRow2 frogY = 324; livesCount -= 1;
#define resetRow3 frogY = 504; livesCount -= 1;
#define resetRow4 frogY = 684; livesCount -= 1;
#define bBlock picture -> bonusBlocks[i]

//GPIO setup macros, borrowed from slide 13 of RPi 2 SNES
#define INP_GPIO(g)			*(GPIO_PTR+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)			*(GPIO_PTR+((g)/10)) |= (1<<(((g)%10)*3))

/* Definitions */
//for general drawings in the program
struct drawing {
	short int color;		//color to be drawn to the screen at a certain location
	int x, y;				//the location of the pixel to be drawn to
	int height; 			//height of the drawing
	int width;				//width of the drawing
	int xOffset, yOffset;	//x and y offset of the drawing
	int visibility;			//will be used for the value pack
};

//modification of the drawing structure. houses 5 drawings in an array to be used later
struct carDrawing {
	struct drawing carsRow[5];
};

//a structure that houses all of the drawings in the program. will be accesed anyttime something needs to be drawn
struct allDrawings {
	struct drawing bg;	//Default background
	struct drawing cg;	//Challenge background
	struct drawing f;	//Frog
	
	struct drawing sd;	//Score
	struct drawing ld;	//Lives
	struct drawing md;	//Moves
	struct drawing td;	//timer
	
	struct drawing win;		//Win screen
	struct drawing lose;	//Lose screen
	
	struct drawing menu;	//menu for start and pause screen
	struct drawing cursor;	//drawing for the cursor in the menu screen
	
	struct drawing score1place;	//ones place of the score
	struct drawing score10place; //tens place of the score
	struct drawing score100place; //hundreds place of the score
	struct drawing score1000place; //thousands place of the score
	
	struct drawing lives1place;		//ones place of the lives
	struct drawing lives10place;	//tens place of the lives
	
	struct drawing moves1place;		//ones place of the moves
	struct drawing moves10place;	//tens place of the moves
	
	struct drawing timer1place;		//ones place of the timer
	struct drawing timer10place;	//tens place of the timer
	
	struct carDrawing firstCars;	//first row of cars
	struct carDrawing row2cars;		//second row of cars
	struct carDrawing thirdCars;	//third row of cars
	struct carDrawing row4cars;		//fourth row of cars
	
	struct carDrawing row6logs;		//sixth row of logs
	struct carDrawing row7logs;		//logs in row 7
	struct carDrawing row8logs;		//logs in row 8
	struct carDrawing row9logs;		//logs in row 9
	
	struct carDrawing row11cars;	//cars in row 11
	struct carDrawing row12cars;	//cars in row 12
	struct carDrawing row13cars;	//cars in row 13
	struct carDrawing row14cars;	//cars in row 14
	
	struct carDrawing row16logs;	//logs in row 16
	struct carDrawing row17logs;	//logs in row 17
	struct carDrawing row18logs;	//logs in row 18
	struct carDrawing row19logs;	//logs in row 19
	
	struct drawing g1Block72;		//grey block to simulate an erase for scores etc
	struct drawing g2Block72;		//grey block to simulate an erase for scores etc
	struct drawing gBlock144;		//grey block to simulate an erase for scores etc
	
	struct drawing bonusBlocks[10];		//value pack implementation
};

//Function headers
int Read_Data();
void init_GPIO(unsigned int *GPIO_PTR);
void Write_Latch(int write);
void Write_Clock(int write);
void wait(int mill);
void movement(int* read_array, struct allDrawings *f);
int read_SNES(int* read_array);
int checkCarCollision(struct allDrawings *picture);
void checkWallCollision(struct allDrawings *picture);
int checkRowCollision(int xOffFrog, struct drawing* cars);
void generateBonus();

void failed(char* id);

//declaring global variables
static unsigned *gpio;		//pointer storing the memory address of the base address of the GPIO Pins

//State flags that determine if a certain event is going on
static char endFlag = 0;			//End of the game, either win or lose condition
static char carCollisionFlag = 0;		//Whether the player has collided with a vehicle or not
static char pauseFlag = 2;			//Whether or not the game is paused
static char cursorPosition = 0;		//Whether the cursor is choosing the play or quit option
static int currentRow = 684;		//Current row of the frog
static int logCollisionFlag = 0;	//
static int logMoveFrogLeft = 0;		//Flag that says the frog must be moving left
static int logMoveFrogRight = 0;	//Flag that says the frog must be moving right

//Player stats
static short int livesCount = 5;	//Number of lives the player has
static int scoresCount = 0;			//Player score
static int movesCount = 99;			//Number of moves before player loses a life
static int timerCount = 90;			//giving the player 90 seconds to play the game

struct fbs framebufferstruct;
void drawPixel(void *pixel);
int frameRate = 1;

//reads the data from the data line
//precondition: none
//postcondition: the value from the data line is returned
int Read_Data() {
	//initializing local variabels
	int read;
	
	//using bit masking and shifting to calculate the value of the buttons
	read = ((gpio[13] >> DAT) & 1);
	
	//return read
	return read;
}

//initializes the GPIO Pins to be used
//precondition: the pointer to the base memory address of the GPIO pins is passed
//postcondition: the correct pins are initialized
void init_GPIO(unsigned int *GPIO_PTR) {
	//Sets pin functions
	OUT_GPIO(CLK);		//clock pin
	OUT_GPIO(LAT);		//latch pin
	INP_GPIO(DAT);		//data pin
}

//sets or clears the latch line depending on the value write passed in
//preconditon: the value write is passed
//post condition: the latch line is either set or cleared
void Write_Latch(int write) {
	
	//switch statement that sets or clears the latch line depending on the value
	//case 0 clears the latch line
	//case 1 sets the latch line
	switch(write) {
		case 0:	gpio[10] = 1 << LAT; break;
		case 1:	gpio[7] = 1 << LAT; break;
		default: /*printf("how'd you get here \n")*/; break;
	}
}

//sets or clears the clock line depending on the value write passed in
//preconditon: the value write is passed
//post condition: the clock line is either set or cleared
void Write_Clock(int write) {
    
	//switch statement that sets or clears the clock line depending on the value
	//case 0 clears the clock line
	//case 1 sets the clock line
	switch(write) {
		case 0:	 gpio[10] = 1 << CLK; break;
		case 1:	 gpio[7] = 1 << CLK; break;
		default: /*printf("how'd you get here \n")*/; break;
	}
}

//waits by a certain amount of microseconds
//precondition: an integer value is passed
//postcondition: the integer value amount of microseconds is waited
void wait(int mill) {
	delayMicroseconds(mill);
}

//the function that incorporates most of the sub functions.
//runs algorithm given in class to read buttons from the SNES controller and to store the values in an array for later use
//precondition: the array samplingButtons is passed to store the values of the buttons pressed
//postcondition: the value of buttonRead is returned
int read_SNES(int *samplingButtons) {
	//This is an implementation of the pseudo code given in 
	//declaring local vars
	int i = 0;
	int buttonRead = 0;
	
	//setting the clock and latch line
	Write_Clock(1);
	Write_Latch(1);
	
	//wait for 12 microseconds
	wait(12);
	
	//clearing the latch line
	Write_Latch(0);
	
	//while there are still buttons to be checked
	//there are only 12 buttons but we account for 16 for future implementations
	while (i < 16) {
		
		wait(6);
		
		//clear the clock line
		Write_Clock(0);
		
		wait(6);
		
		//call read data function and store the returned value into the correct spot in the array of button value memory
		samplingButtons[i] = Read_Data();

		//if the button at the array location is pressed
		if (samplingButtons[i] == 0) {
			//return that a button has been pressed
			buttonRead = 1;
		}
		
		//setting the clock line
		Write_Clock(1);
		i++;
	}

	return buttonRead;
}

//prints messages to the console depending on which buttons are pressed
//precondition: the array of values of buttons is passed
//postcondition: the correct messages are displayed
void movement(int* read_array, struct allDrawings *picture) {
    //declaring local variables
    int i;
    
    //for all the buttons in the array
    for (i = 0; i < 12; i++) {
        
        //if it is pressed, display a message to the console
        if (!read_array[i]) {
            //printf("\nYou have pressed %s \n", buttons[i]);
			
            
//static char* buttons[12] = {"B", "Y", "Select", "Start", "Up", "Down", "Left", "Right", "A", "X", "L", "R"};	//char array that holds all of the types of buttons in the sequence they are read
			
            if (!pauseFlag) {	//If the game isn't paused
				switch (i) {
					case 4: frogY -= 36; movesCount--; break; //Up
					case 5: frogY += 36; movesCount--; break; //Down
					case 6: frogX -= 36; movesCount--; break; //Left
					case 7: frogX += 36; movesCount--; break; //Right
				}
			} else {			//if the game is paused
				switch(i) {
					case 4:	picture -> cursor.yOffset = 430; cursorPosition = 0; break;
					case 5: picture -> cursor.yOffset = 512; cursorPosition = 1; break;
					case 8:
						if (cursorPosition) {
							endFlag = 2;
						} else {
							pauseFlag = 0;
						}
				}
			}
        }
        
    }
	
	 //if the start button is pressed set the flag end to 1 so that the program will end
    if (!read_array[3]) {
        //This is where the pause menu will be activated
		if (pauseFlag) {
			pauseFlag = 0;
			//printf("Unpaused \n");
		} else {
			pauseFlag = 1;
			//printf("Paused \n");
		}
    }
}

//Checks the car collisions based on the y offset of the frog
//precondition: the allDrawings struct is initialized and are given the appropriate offsets
//postcondition: game reacts according to where the collision takes place
int checkCarCollision(struct allDrawings *picture) {
	
	//Check car/log collision here
	
	/*
	 * Time for a LOT of hard coding
	 * This is going to be another massive block of code
	 * may need a helper method just to clean it up LOL
	 */
	 
	//printf("%d \n", frogY);
	
	switch(frogY) {
		//1st challenge <LOGS>
		case 0:		if (currentRow != 0) {
						//printf("1\n");
						currentRow = 0;
					}
					if (checkRowCollision(frogX, picture -> row19logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 1;
						logMoveFrogLeft = 0;
					} else {
						livesCount--;
						resetRow1
					}
					break;
					
		case 36:	if (currentRow != 36) {
						//printf("2\n");
						currentRow = 36;
					}
					if (checkRowCollision(frogX, picture -> row18logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 0;
						logMoveFrogLeft = 1;
					} else {
						livesCount--;
						resetRow1
					}
					break;
					
		case 72:	if (currentRow != 72) {
						//printf("3\n");
						currentRow = 72;
					}
					if (checkRowCollision(frogX, picture -> row17logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 1;
						logMoveFrogLeft = 0;
					} else {
						livesCount--;
						resetRow1
					}
					break;
					
		case 108:	if (currentRow != 108) {
						//printf("4\n");
						currentRow = 108;
					}
					if (checkRowCollision(frogX, picture -> row16logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 0;
						logMoveFrogLeft = 1;
					} else {
						livesCount--;
						resetRow1
					}
					break;
		
		case 144:	if (currentRow != 144) {
						//printf("Safe\n");
						currentRow = 144;
					}
					
					logMoveFrogRight = 0;
					logMoveFrogLeft = 0;
					
					break;
		
		//2nd challenge <CARS>
		case 180:	if (currentRow != 180) {
						//printf("6\n");
						currentRow = 180;
					}
					if (checkRowCollision(frogX, picture -> row14cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow2
					}
					break;
					
		case 216:	if (currentRow != 216) {
						//printf("7\n");
						currentRow = 216;
					}
					if (checkRowCollision(frogX, picture -> row13cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow2
					}
					break;
					
		case 252:	if (currentRow != 252) {
						//printf("8\n");
						currentRow = 252;
					}
					if (checkRowCollision(frogX, picture -> row12cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow2
					}
					break;
					
		case 288:	if (currentRow != 288) {
						//printf("9\n");
						currentRow = 288;
					}
					if (checkRowCollision(frogX, picture -> row11cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow2
					}
					break;
					
		case 324:	if (currentRow != 324) {
						currentRow = 324;
					}
					
					logMoveFrogRight = 0;
					logMoveFrogLeft = 0;
					
					break;
		
		//3rd challenge <LOGS>
		case 360:	if (currentRow != 360) {
						currentRow = 360;
					}
					if (checkRowCollision(frogX, picture -> row9logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 1;
						logMoveFrogLeft = 0;
					} else {
						livesCount--;
						resetRow3
					}
					break;
					
		case 396:	if (currentRow != 396) {
						currentRow = 396;
					}
					if (checkRowCollision(frogX, picture -> row8logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 0;
						logMoveFrogLeft = 1;
					} else {
						livesCount--;
						resetRow3
					}
					break;
					
		case 432:	if (currentRow != 432) {
						currentRow = 432;
					}
					if (checkRowCollision(frogX, picture -> row7logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 1;
						logMoveFrogLeft = 0;
					} else {
						livesCount--;
						resetRow3
					}
					break;
					
		case 468:	if (currentRow != 468) {
						//printf("14\n");
						currentRow = 468;
					}
					if (checkRowCollision(frogX, picture -> row6logs.carsRow)) {
						logCollisionFlag = 1;
						logMoveFrogRight = 0;
						logMoveFrogLeft = 1;
					} else {
						livesCount--;
						resetRow3
					}
					break;
					
		case 504:	if (currentRow != 504) {
						currentRow = 504;
					}
					
					logMoveFrogRight = 0;
					logMoveFrogLeft = 0;
					
					break;
		
		//4th challenge <CARS>
		case 540:	if (currentRow != 540) {
						currentRow = 540;
					}
					if (checkRowCollision(frogX, picture -> row4cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow4
					}
					break;
					
		case 576:	if (currentRow != 576) {
						currentRow = 576;
					}
					if (checkRowCollision(frogX, picture -> thirdCars.carsRow)) {
						carCollisionFlag = 1;
						resetRow4
					}
					break;
					
		case 612:	if (currentRow != 612) {
						currentRow = 612;
					}
					if (checkRowCollision(frogX, picture -> row2cars.carsRow)) {
						carCollisionFlag = 1;
						resetRow4
					}
					break;
					
		case 648:	if (currentRow != 648) {
						currentRow = 648;
					}
					if (checkRowCollision(frogX, picture -> firstCars.carsRow)) {
						carCollisionFlag = 1;
						resetRow4
					}
					break;
	}
	
	return 0;
}

//Checks whether a collision occured with the cars in the row specified
//precondition: xOffset of frog is set and cars arrat is initialized
//postcondition: the correct messages are displayed
int checkRowCollision(int xOffFrog, struct drawing* cars) {
	int i;
	
	//Goes through each vehicle in a row of cars
	for (i = 0; i < 5; i++) {
		//Checks left side for collision
		if (xOffFrog >= cars[i].xOffset && xOffFrog <= cars[i].xOffset + cars[i].width) {
			return 1;
		}
		//Checks right side for collision
		if (xOffFrog + 36 >= cars[i].xOffset && xOffFrog + 36 <= cars[i].xOffset + cars[i].width) {
			return 1;
		}
	}
	
	logMoveFrogRight = 0;
	logMoveFrogLeft = 0;
	
	carCollisionFlag = 0;
	logCollisionFlag = 0;
	return 0;
}

//Checks if the user is attempting to leave the play area
//precondition: borders of play area are defined and the frog is moved to a specific pair of x,y offsets
//postcondition: if the frog is outside, push them back into the play area
void checkWallCollision(struct allDrawings *picture) {
	
	int i;
	
	//check wall collision here
	if (frogY < 0) {
		//printf("Collided with top wall \n");
		//At this point, they technically win lols
		endFlag = 1;
		frogY += 36;
	}
	
	if (frogY > 684) {
		//printf("Collided with bottom wall \n");
		frogY -= 36;
	}
	
	if (frogX < 0) {
		//printf("Collided with left wall \n");
		frogX += 36;
	}
	
	if (frogX > 828) {
		//printf("Collided with right wall \n");
		frogX -= 36;
	}
	
	//Check bonus pack collision here
	for (i = 0; i < 10; i++) {
		if (bBlock.visibility) {
			if (frogX >= bBlock.xOffset && frogX <= (bBlock.xOffset + bBlock.width)) {	//Check x coordinates
				if (frogY == bBlock.yOffset) {	//Check the y coordinates
					//Makes block invisible and generates a random bonus
					bBlock.visibility = 0;
					generateBonus();
				}
			}
			//Checks right side for collision
			if (frogX + 36 >= bBlock.xOffset && frogX + 36 <= bBlock.xOffset + bBlock.width) { //Check x coordinates
				if (frogY == bBlock.yOffset) {	//check the y coordinates
					//Makes block invisible and generates a random bonus
					bBlock.visibility = 0;
					generateBonus();
				}
			}
		}
	}
}

//Main function for controller controller thread. Used to read user input and have the game react
//with the correct action
//precondition: SNES controller is connected to RPi, and GPIO values are initialized to read the controller input
//postcondition: user is allowed to interact with the game world
void *controllerMain(void *in) {
	
	struct allDrawings *f = in;
	
	// get gpio pointer
    unsigned int *gpioPtr = getGPIOPtr();
	
	//Plenty of the code will be borrowed from the files given to us by the
	//prof and TAs
	
	//array that stores the values read from the controller
	int read_array[16];
	
	gpio = gpioPtr;
	
	gpio[7] = 0;
	gpio[10] = 0;
	gpio[13] = 0;
	
	//initializing the GPIO Pins
	init_GPIO(gpioPtr);
	
	//while the start button is not being pressed
	while (!endFlag) {
		
		//while a button is not pressed
		while (!read_SNES(read_array) && !endFlag);
		
		//React to buttons pressed
		movement(read_array, f);
		
		//wait before moving again
		wait(250000);
	}
	
	pthread_exit(0);
}

void failed(char* id) {
	//printf("Failed to create thread %s \n", id);
}

//draws the picture given in the parameter to the screen at a given offset and with given color
void draw(struct drawing *toBeDrawed, short int *drawStructPtr) {
	
	//initalizing local variables
	int i=0;
	//unsigned int quarter,byte,word; idk why I kept it for this long if it's not being used lol
	int height = toBeDrawed->height;
	int width = toBeDrawed->width;
	
	//calculating height and width after offset
	height = height + toBeDrawed->yOffset;
	width = width + toBeDrawed->xOffset;
	
	long int location;
	
	//for loop that cycles through the drawing. Draws the pixels accordingly
	for (int y = toBeDrawed->yOffset; y < height; y++)
	{
		for (int x = toBeDrawed->xOffset; x < width; x++)
		{	
			toBeDrawed->color = drawStructPtr[i]; 
			toBeDrawed->x = x;
			toBeDrawed->y = y;
	
			location =(toBeDrawed->x +framebufferstruct.xOff) * (framebufferstruct.bits/8) +
            (toBeDrawed->y+framebufferstruct.yOff) * framebufferstruct.lineLength;
			*((unsigned short int*)(framebufferstruct.fptr + location)) = toBeDrawed->color;
				
			i++;	
		}
	}
}

//calculates which digit to draw at which location depending on the values of the lives, scors and moves.
//draws to the correct location accordingly
void drawData(struct allDrawings *picture, short int *numberPtr[]) {
		
	//will be used to determine which digit will be drawn
	int remainder = 0;
	
	//Draw player stats at top right of display
	if (!endFlag) {	
		//calculate and draw the thousands place
		remainder = scoresCount / 1000;
		draw(&(picture->score1000place), numberPtr[remainder]);
		
		//calculate the hundreds place and draw it
		remainder = scoresCount % 1000;
		remainder = remainder / 100;
		draw(&(picture->score100place), numberPtr[remainder]);
		
		//calculate the tens place and draw it
		remainder = scoresCount % 100;
		remainder = remainder / 10;
		draw(&(picture->score10place), numberPtr[remainder]);
		
		//calculate the ones place and draw it
		remainder = scoresCount % 10;
		draw(&(picture->score1place), numberPtr[remainder]);
		
		//calculate the tens place and draw it	
		remainder = livesCount % 100;
		remainder = remainder / 10;
		draw(&(picture->lives10place), numberPtr[remainder]);
		
		//calculate the ones place and draw it
		remainder = livesCount % 10;
		draw(&(picture->lives1place), numberPtr[remainder]);
	
		//calculate the tens place and draw it
		remainder = movesCount % 100;
		remainder = remainder / 10;
		draw(&(picture->moves10place), numberPtr[remainder]);
	
		//calculate the ones place and draw it
		remainder = movesCount % 10;
		draw(&(picture->moves1place), numberPtr[remainder]);
	
		//calculate the tens place and draw it
		remainder = timerCount % 100;
		remainder = remainder / 10;
		draw(&(picture->timer10place), numberPtr[remainder]);
	
		//calculate the ones place and draw it
		remainder = timerCount % 10;
		draw(&(picture->timer1place), numberPtr[remainder]);
	} else {	//Draws player score at the bottom left of screen
		((struct allDrawings*)picture)->score1000place.height = 36;
		((struct allDrawings*)picture)->score1000place.width = 36;
		((struct allDrawings*)picture)->score1000place.xOffset = 36;
		((struct allDrawings*)picture)->score1000place.yOffset = 696;
	
		((struct allDrawings*)picture)->score100place.height = 36;
		((struct allDrawings*)picture)->score100place.width = 36;
		((struct allDrawings*)picture)->score100place.xOffset = 72;
		((struct allDrawings*)picture)->score100place.yOffset = 696;
	
		((struct allDrawings*)picture)->score10place.height = 36;
		((struct allDrawings*)picture)->score10place.width = 36;
		((struct allDrawings*)picture)->score10place.xOffset = 108;
		((struct allDrawings*)picture)->score10place.yOffset = 696;
	
		((struct allDrawings*)picture)->score1place.height = 36;
		((struct allDrawings*)picture)->score1place.width = 36;
		((struct allDrawings*)picture)->score1place.xOffset = 144;
		((struct allDrawings*)picture)->score1place.yOffset = 696;
		
		//calculate and draw the thousands place
		remainder = scoresCount / 1000;
		draw(&(picture->score1000place), numberPtr[remainder]);
		
		//calculate the hundreds place and draw it
		remainder = scoresCount % 1000;
		remainder = remainder / 100;
		draw(&(picture->score100place), numberPtr[remainder]);
		
		//calculate the tens place and draw it
		remainder = scoresCount % 100;
		remainder = remainder / 10;
		draw(&(picture->score10place), numberPtr[remainder]);
		
		//calculate the ones place and draw it
		remainder = scoresCount % 10;
		draw(&(picture->score1place), numberPtr[remainder]);
	}
}

//used to initialize the cars and log sequences for each row.
// the values assigned vary depending on size of the drawing and the direction the sequence is moving
void initializeCarLog(struct carDrawing * car, int carSize, int rowNum, int direction) {
	int a;
	
	//for loop assigning heightm width and yoffset to all cars in the sequence
	for (a = 0; a < 5; a++) {
		car->carsRow[a].height = 36;
		car->carsRow[a].width = carSize * 36;		
		car->carsRow[a].yOffset = rowNum * 36;
	}
	
	//direction guideline 0 = right to left, 1 = left to right
	switch (direction) {
		case 0: 
			car->carsRow[0].xOffset = 936;
			car->carsRow[1].xOffset = 1080;
			car->carsRow[2].xOffset = 1260;
			car->carsRow[3].xOffset = 1476;
			car->carsRow[4].xOffset = 1764;
		break;
		case 1: 
			car->carsRow[0].xOffset = -72;
			car->carsRow[1].xOffset = -216;
			car->carsRow[2].xOffset = -396;
			car->carsRow[3].xOffset = -540;
			car->carsRow[4].xOffset = -720;				
		break;
	}
	
}

//used to initialize all of the drawings in the program
void initializeAllDrawings(struct allDrawings * picture) {
	//initializing the menu
	((struct allDrawings*)picture)->menu.height = 768;
	((struct allDrawings*)picture)->menu.width = 1024;
	((struct allDrawings*)picture)->menu.xOffset = 0;
	((struct allDrawings*)picture)->menu.yOffset = 0;
		
	//initializing the cursor
	((struct allDrawings*)picture)->cursor.height = 36;
	((struct allDrawings*)picture)->cursor.width = 36;
	((struct allDrawings*)picture)->cursor.xOffset = 180;
	((struct allDrawings*)picture)->cursor.yOffset = 430;
	
	//initializing score label and the places of the digits
	((struct allDrawings*)picture)->sd.height = 36;
	((struct allDrawings*)picture)->sd.width = 160;
	((struct allDrawings*)picture)->sd.xOffset = 864;
	((struct allDrawings*)picture)->sd.yOffset = 0;
	
	((struct allDrawings*)picture)->score1000place.height = 36;
	((struct allDrawings*)picture)->score1000place.width = 36;
	((struct allDrawings*)picture)->score1000place.xOffset = 864;
	((struct allDrawings*)picture)->score1000place.yOffset = 36;
	
	((struct allDrawings*)picture)->score100place.height = 36;
	((struct allDrawings*)picture)->score100place.width = 36;
	((struct allDrawings*)picture)->score100place.xOffset = 900;
	((struct allDrawings*)picture)->score100place.yOffset = 36;
	
	((struct allDrawings*)picture)->score10place.height = 36;
	((struct allDrawings*)picture)->score10place.width = 36;
	((struct allDrawings*)picture)->score10place.xOffset = 936;
	((struct allDrawings*)picture)->score10place.yOffset = 36;
	
	((struct allDrawings*)picture)->score1place.height = 36;
	((struct allDrawings*)picture)->score1place.width = 36;
	((struct allDrawings*)picture)->score1place.xOffset = 972;
	((struct allDrawings*)picture)->score1place.yOffset = 36;
	
	//initializing lives label and the places of the digits
	((struct allDrawings*)picture)->ld.height = 36;
	((struct allDrawings*)picture)->ld.width = 160;
	((struct allDrawings*)picture)->ld.xOffset =864;
	((struct allDrawings*)picture)->ld.yOffset = 72;
	
	((struct allDrawings*)picture)->lives10place.height = 36;
	((struct allDrawings*)picture)->lives10place.width = 36;
	((struct allDrawings*)picture)->lives10place.xOffset = 864;
	((struct allDrawings*)picture)->lives10place.yOffset = 108;
	
	((struct allDrawings*)picture)->lives1place.height = 36;
	((struct allDrawings*)picture)->lives1place.width = 36;
	((struct allDrawings*)picture)->lives1place.xOffset = 900;
	((struct allDrawings*)picture)->lives1place.yOffset = 108;
	
	//initializing moves label and the places of the digits
	((struct allDrawings*)picture)->md.height = 36;
	((struct allDrawings*)picture)->md.width = 160;
	((struct allDrawings*)picture)->md.xOffset = 864;
	((struct allDrawings*)picture)->md.yOffset = 144;
	
	((struct allDrawings*)picture)->moves10place.height = 36;
	((struct allDrawings*)picture)->moves10place.width = 36;
	((struct allDrawings*)picture)->moves10place.xOffset = 864;
	((struct allDrawings*)picture)->moves10place.yOffset = 180;
	
	((struct allDrawings*)picture)->moves1place.height = 36;
	((struct allDrawings*)picture)->moves1place.width = 36;
	((struct allDrawings*)picture)->moves1place.xOffset = 900;
	((struct allDrawings*)picture)->moves1place.yOffset = 180;
	
	//initializing timer label and the places of the digits
	((struct allDrawings*)picture)->td.height = 36;
	((struct allDrawings*)picture)->td.width = 160;
	((struct allDrawings*)picture)->td.xOffset = 864;
	((struct allDrawings*)picture)->td.yOffset = 216;
	
	((struct allDrawings*)picture)->timer10place.height = 36;
	((struct allDrawings*)picture)->timer10place.width = 36;
	((struct allDrawings*)picture)->timer10place.xOffset = 864;
	((struct allDrawings*)picture)->timer10place.yOffset = 252;
	
	((struct allDrawings*)picture)->timer1place.height = 36;
	((struct allDrawings*)picture)->timer1place.width = 36;
	((struct allDrawings*)picture)->timer1place.xOffset = 900;
	((struct allDrawings*)picture)->timer1place.yOffset = 252;
	
	//initializing the gray background
	((struct allDrawings*)picture)->bg.height = 768;
	((struct allDrawings*)picture)->bg.width = 1024;
	((struct allDrawings*)picture)->bg.xOffset = 0;
	((struct allDrawings*)picture)->bg.yOffset = 0;
	
	//initializing the win screen
	((struct allDrawings*)picture)->win.height = 768;
	((struct allDrawings*)picture)->win.width = 1024;
	((struct allDrawings*)picture)->win.xOffset = 0;
	((struct allDrawings*)picture)->win.yOffset = 0;
	
	//initializing the lose screen
	((struct allDrawings*)picture)->lose.height = 768;
	((struct allDrawings*)picture)->lose.width = 1024;
	((struct allDrawings*)picture)->lose.xOffset = 0;
	((struct allDrawings*)picture)->lose.yOffset = 0;
	
	//initializing challenge background
	((struct allDrawings*)picture)->cg.height = 720;
	((struct allDrawings*)picture)->cg.width = 864;
	((struct allDrawings*)picture)->cg.xOffset = 0;
	((struct allDrawings*)picture)->cg.yOffset = 0;
	
	//initializing challenge background
	((struct allDrawings*)picture)->f.height = 36;
	((struct allDrawings*)picture)->f.width = 36;
	((struct allDrawings*)picture)->f.xOffset = 144;
	((struct allDrawings*)picture)->f.yOffset = 684;
	
	//initializing the grey block 36 * 72
	((struct allDrawings*)picture)->g1Block72.height = 36;
	((struct allDrawings*)picture)->g1Block72.width = 72;
	((struct allDrawings*)picture)->g1Block72.xOffset = 864;
	((struct allDrawings*)picture)->g1Block72.yOffset = 108;
	
	//initializing the grey block 36 * 72
	((struct allDrawings*)picture)->g2Block72.height = 36;
	((struct allDrawings*)picture)->g2Block72.width = 72;
	((struct allDrawings*)picture)->g2Block72.xOffset = 864;
	((struct allDrawings*)picture)->g2Block72.yOffset = 180;
	
	//initializing the grey block 36 * 144
	((struct allDrawings*)picture)->gBlock144.height = 36;
	((struct allDrawings*)picture)->gBlock144.width = 144;
	((struct allDrawings*)picture)->gBlock144.xOffset = 864;
	((struct allDrawings*)picture)->gBlock144.yOffset = 36;
	
	//initializing the cars and logs
	initializeCarLog(&(((struct allDrawings*)picture)->firstCars), 2, 18, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row2cars), 3, 17, 1);
	initializeCarLog(&(((struct allDrawings*)picture)->thirdCars), 2, 16, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row4cars), 3, 15, 1);
	
	initializeCarLog(&(((struct allDrawings*)picture)->row6logs), 2, 13, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row7logs), 3, 12, 1);
	initializeCarLog(&(((struct allDrawings*)picture)->row8logs), 2, 11, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row9logs), 3, 10, 1);
	
	initializeCarLog(&(((struct allDrawings*)picture)->row11cars), 2, 8, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row12cars), 3, 7, 1);
	initializeCarLog(&(((struct allDrawings*)picture)->row13cars), 2, 6, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row14cars), 3, 5, 1);
	
	initializeCarLog(&(((struct allDrawings*)picture)->row16logs), 2, 3, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row17logs), 3, 2, 1);
	initializeCarLog(&(((struct allDrawings*)picture)->row18logs), 2, 1, 0);
	initializeCarLog(&(((struct allDrawings*)picture)->row19logs), 3, 0, 1);
	
	srand(time(0));
	
	//initializing the value packs
	int a;
	for (a = 0; a < 10; a++) {
		((struct allDrawings*)picture)->bonusBlocks[a].height = 36;
		((struct allDrawings*)picture)->bonusBlocks[a].width = 36;
		((struct allDrawings*)picture)->bonusBlocks[a].xOffset = 36 * (rand() % 20);
		((struct allDrawings*)picture)->bonusBlocks[a].yOffset = 36 * (rand() % 20);
		((struct allDrawings*)picture)->bonusBlocks[a].visibility = 1;		
	}
}

void moveCarLogLeft(struct carDrawing * car, short int * toBeDrawn, struct drawing * frog, short int * toBeDrawn2) {
	
	//decrement the x offset of all of the cars in the sequence by 2
	car->carsRow[0].xOffset -= 2;
	car->carsRow[1].xOffset -= 2;
	car->carsRow[2].xOffset -= 2;
	car->carsRow[3].xOffset -= 2;
	car->carsRow[4].xOffset -= 2;
	
	//resets the values if the cars or logs are out of bounds
	if (car->carsRow[0].xOffset <= -540) {
		car->carsRow[0].xOffset = 936;
	}
		
	if (car->carsRow[1].xOffset <= -396) {
		car->carsRow[1].xOffset = 1080;
	}
	
	if (car->carsRow[2].xOffset <= -216) {
		car->carsRow[2].xOffset = 1260;
	}
	
	if (car->carsRow[3].xOffset <= 0) {
		car->carsRow[3].xOffset = 1476;
	}
	
	if (car->carsRow[4].xOffset <= 288) {
		car->carsRow[4].xOffset = 1764;
	}
	
	int a;
	
	//draws the cars in the sequence if it is in bounds
	for (a = 0; a < 5; a++) {
		if (car->carsRow[a].xOffset <= 792 && car->carsRow[a].xOffset > 0) {
			draw(&(car->carsRow[a]), toBeDrawn);
		}
	}
}

void moveCarLogRight(struct carDrawing * car, short int * toBeDrawn, struct drawing * frog, short int * toBeDrawn2) {
	
	//increment the x offset in cars of the sequence by 2
	car->carsRow[0].xOffset += 2;
	car->carsRow[1].xOffset += 2;
	car->carsRow[2].xOffset += 2;
	car->carsRow[3].xOffset += 2;
	car->carsRow[4].xOffset += 2;
	
	//resets the location of the cars if they are out of bounds
	if (car->carsRow[0].xOffset >= 1584) {
		car->carsRow[0].xOffset = -72;
	}
	
	if (car->carsRow[1].xOffset >= 1404) {
		car->carsRow[1].xOffset = -216;
	}
	
	if (car->carsRow[2].xOffset >= 1260) {
		car->carsRow[2].xOffset = -396;
	}
	
	if (car->carsRow[3].xOffset >= 1080) {
		car->carsRow[3].xOffset = -540;
	}
	
	if (car->carsRow[4].xOffset >= 936) {
		car->carsRow[4].xOffset = -720;
	}
	
	int a;
	
	//draws the cars if they are in bounds
	for (a = 0; a < 5; a++) {
		if (car->carsRow[a].xOffset <= 756 && car->carsRow[a].xOffset > 0) {
			draw(&(car->carsRow[a]), toBeDrawn);
		}
	}
}

//generates a bonus and applys it if the player hits a bonus block
void generateBonus() {
	int i = rand() % 3;
	
	switch (i) {
		case 0: scoresCount += 100; break;
		case 1: livesCount+= 1; break;
		case 2: movesCount += 5; break;
	}
}

void* drawThread(void * picture) {
	
	//declaring all of the drawings to be used in the program
	short int *car2Ptr=(short int *) vehicle2.pixel_data;
	short int *car3Ptr=(short int *) vehicle3.pixel_data;
	short int *log2Ptr=(short int *) log2.pixel_data;
	short int *log3Ptr=(short int *) log3.pixel_data;
	short int *defaultBackPtr=(short int *) defaultBack.pixel_data;
	short int *challengePtr=(short int *) challenge.pixel_data;
	short int *frogPtr = (short int*) frog.pixel_data;
	short int *scorePtr = (short int*) score.pixel_data;
	short int *livesPtr = (short int*) lives.pixel_data;
	short int *movesPtr = (short int*) moves.pixel_data;
	short int *gBlock72Ptr = (short int*) block72.pixel_data;
	short int *gBlock144Ptr = (short int*) block144.pixel_data;
	short int *valuePackPtr = (short int*) valuePack.pixel_data;
	short int *timerPtr = (short int*) timer.pixel_data;
	short int *menuPtr = (short int*) menu.pixel_data;
	short int *cursorPtr = (short int*) cursor.pixel_data;
	
	//the number drawings are stored in an array for later use
	short int *numberPtr[10];
	numberPtr[0] = (short int*) zero.pixel_data;
	numberPtr[1] = (short int*) one.pixel_data;
	numberPtr[2] = (short int*) two.pixel_data;
	numberPtr[3] = (short int*) three.pixel_data;
	numberPtr[4] = (short int*) four.pixel_data;
	numberPtr[5] = (short int*) five.pixel_data;
	numberPtr[6] = (short int*) six.pixel_data;
	numberPtr[7] = (short int*) seven.pixel_data;
	numberPtr[8] = (short int*) eight.pixel_data;
	numberPtr[9] = (short int*) nine.pixel_data;
	
	//function call to set all of the values of the drawings in the structure
	//values are hard coded in this function
	initializeAllDrawings(((struct allDrawings*)picture));
	
	//drawing the gray background and labels for player data
	//these do not need to be redrawn so these are called outside of the loop
	draw(&(((struct allDrawings*)picture)->bg), defaultBackPtr);
	
	//checking collision with the wall
	checkWallCollision(picture);
	
	//counter
	int a;
	
	//while the game has not ended
	while (!endFlag) {
		//while the game is not paused
		while(pauseFlag && !endFlag) {
			draw(&(((struct allDrawings*)picture)->menu), menuPtr);
			draw(&(((struct allDrawings*)picture)->cursor), cursorPtr);
			//be like if down or up is pressed, modify the yoffset of the cursor accordingly
		}
		
		draw(&(((struct allDrawings*)picture)->ld), livesPtr);
		draw(&(((struct allDrawings*)picture)->md), movesPtr);
		draw(&(((struct allDrawings*)picture)->sd), scorePtr);
		draw(&(((struct allDrawings*)picture)->td), timerPtr);
		
		//reset the colors where the player data would be put
		draw(&(((struct allDrawings*)picture)->g1Block72), gBlock72Ptr);
		draw(&(((struct allDrawings*)picture)->g2Block72), gBlock72Ptr);
		draw(&(((struct allDrawings*)picture)->gBlock144), gBlock144Ptr);
		
		//updatine lives, scores, and moves
		drawData(((struct allDrawings*)picture), numberPtr);
		
		//Draws base layer
		draw(&(((struct allDrawings*)picture)->cg), challengePtr);
		
		//drawing the bonus blocks into the screen
		for (a = 0; a < 10; a++) {
			//if visibility is set to be true
			if (((struct allDrawings*)picture)->bonusBlocks[a].visibility == 1) {
				draw(&(((struct allDrawings*)picture)->bonusBlocks[a]), valuePackPtr);
			}
		}
		
		//moving the bottom 4 rows of cars in their respective directions and drawing the update
		moveCarLogLeft(&(((struct allDrawings*)picture)->firstCars), car2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row2cars), car3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogLeft(&(((struct allDrawings*)picture)->thirdCars), car2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row4cars), car3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		
		//moving the bottom 4 rows of logs in their respective directions and drawing the update
		moveCarLogLeft(&(((struct allDrawings*)picture)->row6logs), log2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row7logs), log3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogLeft(&(((struct allDrawings*)picture)->row8logs), log2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row9logs), log3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		
		//moving the top 4 rows of cars in their respective directions and drawing the update
		moveCarLogLeft(&(((struct allDrawings*)picture)->row11cars), car2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row12cars), car3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogLeft(&(((struct allDrawings*)picture)->row13cars), car2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row14cars), car3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		
		//moving the top 4 rows of logs in their respective directions and drawing the update
		moveCarLogLeft(&(((struct allDrawings*)picture)->row16logs), log2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row17logs), log3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogLeft(&(((struct allDrawings*)picture)->row18logs), log2Ptr, &(((struct allDrawings*)picture)->f), frogPtr);
		moveCarLogRight(&(((struct allDrawings*)picture)->row19logs), log3Ptr, &(((struct allDrawings*)picture)->f), frogPtr);

		//logic for if the frog is on a log. The frog is adjusted to move left or right accordingly
		if (logMoveFrogRight && logCollisionFlag) {
			((struct allDrawings*)picture) -> f.xOffset += 2;
		}

		if (logMoveFrogLeft && logCollisionFlag) {
			((struct allDrawings*)picture) -> f.xOffset -= 2;
		}

		//draw the updated location of the frog
		draw(&(((struct allDrawings*)picture)->f), frogPtr);

		//delaying for a certain amount of time to allow for visual updates
		delayMicroseconds(1000);
	}
	
	//exiting the thread once the end condition is met
	pthread_exit(0);
}

//Timer thread, used to decrement timerCount
//independent of other threads
void* timerThread(void* param) {
	
	while (!endFlag) {
		
		while(pauseFlag && !endFlag);
		
		timerCount--;
		sleep(1);
	}
	
	pthread_exit(0);
}


/* main function */
int main(){

	/* initialize + get FBS */
	framebufferstruct = initFbInfo();
	
	//Things that will be drawn once the game ends
	struct allDrawings *picture = (struct allDrawings *)malloc(sizeof(struct allDrawings));
	short int *defaultBackPtr=(short int *) defaultBack.pixel_data;
	short int *winPtr=(short int *) winEnd.pixel_data;
	short int *losePtr=(short int *) loseEnd.pixel_data;
	short int *scorePtr = (short int*) score.pixel_data;
		
	//the number drawings are stored in an array for later use
	short int *numberPtr[10];
	numberPtr[0] = (short int*) zero.pixel_data;
	numberPtr[1] = (short int*) one.pixel_data;
	numberPtr[2] = (short int*) two.pixel_data;
	numberPtr[3] = (short int*) three.pixel_data;
	numberPtr[4] = (short int*) four.pixel_data;
	numberPtr[5] = (short int*) five.pixel_data;
	numberPtr[6] = (short int*) six.pixel_data;
	numberPtr[7] = (short int*) seven.pixel_data;
	numberPtr[8] = (short int*) eight.pixel_data;
	numberPtr[9] = (short int*) nine.pixel_data;
		
    pthread_t drawT;
	pthread_t playerMove;
	pthread_t playerTimer;
	
	//printf("bruh\n");
    //launch the main thread
    pthread_create(&drawT, NULL, drawThread, picture);
    pthread_create(&playerMove, NULL, controllerMain, picture);
    pthread_create(&playerTimer, NULL, timerThread, picture);
	
	while(!endFlag) {
		//This just runs and checks for car collisions
		checkCarCollision(picture);
		checkWallCollision(picture);
		
		if (livesCount < 0) {
			endFlag = 2;
		}
		
		if (movesCount <= 0) {
			livesCount--;
			movesCount = 99;
		}
		
		if (timerCount <= 0) {
			endFlag = 2;
		}
		
		scoresCount = timerCount + (livesCount * 10) + movesCount;
	}

    //join back the main game thread once it cancels itself.
	pthread_join(drawT, NULL);
	pthread_join(playerMove, NULL);
	pthread_join(playerTimer, NULL);
	
	draw(&(((struct allDrawings*)picture)->bg), defaultBackPtr);
	
	//Here we'll get the win or lose window to be drawn
	//This ensure all the threads are joined before we
	//try to draw anything else
	if (endFlag == 1) {
		//Draw the win screen
		draw(&(((struct allDrawings*)picture)->win), winPtr);
	} else if (endFlag == 2){
		//draw the lose screen
		draw(&(((struct allDrawings*)picture)->lose), losePtr);
	} else {
		//Maybe just draw a quit screen?
	}
	
	//Draws the player's score on the win or lose screen
	((struct allDrawings*)picture)->sd.xOffset = 36;
	((struct allDrawings*)picture)->sd.yOffset = 660;
	draw(&(((struct allDrawings*)picture)->sd), scorePtr);
	drawData(((struct allDrawings*)picture), numberPtr);
	
	/* free pixel's allocated memory */
	free(picture);
	picture = NULL;
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	
	return 0;
}

