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
#include "images/vehicle2.h"
#include "images/lives.h"
#include "images/score.h"
#include "images/moves.h"
#include <unistd.h>
#include <wiringPi.h>
#include "initGPIO.h"

//Defines the pin nunber for CLK, DAT, and LAT
#define LAT 9
#define DAT 10
#define CLK 11
#define frogY picture -> f.yOffset
#define frogX picture -> f.xOffset

//GPIO setup macros, borrowed from slide 13 of RPi 2 SNES
#define INP_GPIO(g)			*(GPIO_PTR+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)			*(GPIO_PTR+((g)/10)) |= (1<<(((g)%10)*3))

/* Definitions */
struct drawing {
	short int color;
	int x, y;
	int height; 
	int width;
	int xOffset, yOffset;
};

struct carDrawing {
	struct drawing carsRow[5];
};

struct allDrawings {
	struct drawing bg;	//Default background
	struct drawing cg;	//Challenge background
	struct drawing f;	//Frog
	struct drawing sd;	//Score
	struct drawing ld;	//Lives
	struct drawing md;	//Moves
	struct carDrawing firstCars;
	struct carDrawing thirdCars;
	struct carDrawing row6logs;
	struct carDrawing row8logs;
	struct carDrawing row11cars;
	struct carDrawing row13cars;
	struct carDrawing row16logs;
	struct carDrawing row18logs;
	
};

int Read_Data();
void init_GPIO(unsigned int *GPIO_PTR);
void Write_Latch(int write);
void Write_Clock(int write);
void wait(int mill);
void movement(int* read_array, struct allDrawings *f);
int read_SNES(int* read_array);
int checkCarCollision(struct allDrawings *picture);
void checkWallCollision(struct allDrawings *picture);

void failed(char* id);

//declaring global variables
static unsigned *gpio;		//pointer storing the memory address of the base address of the GPIO Pins

//State flags that determine if a certain event is going on
static char endFlag = 0;			//End of the game, either win or lose condition
static char collisionFlag = 0;		//Whether the player has collided with a vehicle or not
static char pauseFlag = 0;			//Whether or not the game is paused
static int currentRow = 684;		//Current row of the frog

static short int livesCount = 3;	//Number of lives the player has
static int scoresCount = 0;			//Player score
static int movesCount = 50;			//Number of moves before player loses a life

//static char* buttons[12] = {"B", "Y", "Select", "Start", "Up", "Down", "Left", "Right", "A", "X", "L", "R"};	//char array that holds all of the types of buttons in the sequence they are read
//char* tempBuffer;

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
			
            //f->xOffset = 828;
            //f->yOffset = 684;
            if (!pauseFlag) {
				switch (i) {
					case 4: frogY -= 36; break; //Up
					case 5: frogY += 36; break; //Down
					case 6: frogX -= 36; break; //Left
					case 7: frogX += 36; break; //Right
				}
			}
        }
        
    }
	
	checkWallCollision(picture);
	
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

int checkCarCollision(struct allDrawings *picture) {
	
	//Check car/log collision here
	
	/*
	 * Time for a LOT of hard coding
	 * This is going to be another massive block of code
	 * may need a helper method just to clean it up LOL
	 */
	 
	//printf("%d \n", frogY);
	
	switch(frogY) {
		//1st challenge
		case 0:		if (currentRow != 0) {
						printf("1\n");
						currentRow = 0;
					}
					break;
					
		case 36:	if (currentRow != 36) {
						printf("2\n");
						currentRow = 36;
					}
					break;
					
		case 72:	if (currentRow != 72) {
						printf("3\n");
						currentRow = 72;
					}
					break;
					
		case 108:	if (currentRow != 108) {
						printf("4\n");
						currentRow = 108;
					}
					break;
		
		case 144:	if (currentRow != 144) {
						printf("5\n");
						currentRow = 144;
					}
					break;
		
		//2nd challenge
		case 180:	if (currentRow != 180) {
						printf("6\n");
						currentRow = 180;
					}
					break;
					
		case 216:	if (currentRow != 216) {
						printf("7\n");
						currentRow = 216;
					}
					break;
					
		case 252:	if (currentRow != 252) {
						printf("8\n");
						currentRow = 252;
					}
					break;
					
		case 288:	if (currentRow != 288) {
						printf("9\n");
						currentRow = 288;
					}
					break;
					
		case 324:	if (currentRow != 324) {
						printf("10\n");
						currentRow = 324;
					}
					break;
		
		//3rd challenge
		case 360:	if (currentRow != 360) {
						printf("11\n");
						currentRow = 360;
					}
					break;
					
		case 396:	if (currentRow != 396) {
						printf("12\n");
						currentRow = 396;
					}
					break;
					
		case 432:	if (currentRow != 432) {
						printf("13\n");
						currentRow = 432;
					}
					break;
					
		case 468:	if (currentRow != 468) {
						printf("14\n");
						currentRow = 468;
					}
					break;
					
		case 504:	if (currentRow != 504) {
						printf("15\n");
						currentRow = 504;
					}
					break;
		
		//4th challenge
		case 540:	if (currentRow != 540) {
						printf("16\n");
						currentRow = 540;
					}
					break;
					
		case 576:	if (currentRow != 576) {
						printf("17\n");
						currentRow = 576;
					}
					break;
					
		case 612:	if (currentRow != 612) {
						printf("18\n");
						currentRow = 612;
					}
					break;
					
		case 648:	if (currentRow != 648) {
						printf("19\n");
						currentRow = 648;
					}
					break;
	}
	
	return 0;
}

void checkWallCollision(struct allDrawings *picture) {
	
	//check wall collision here
	
	if (frogY < 0) {
		printf("Collided with top wall \n");
		frogY += 36;
	}
	
	if (frogY > 684) {
		printf("Collided with bottom wall \n");
		//At this point, they technically win lols
		frogY -= 36;
	}
	
	if (frogX < 0) {
		printf("Collided with left wall \n");
		frogX += 36;
	}
	
	if (frogX > 828) {
		printf("Collided with right wall \n");
		frogX -= 36;
	}
}

void *controllerMain(void *in) {
	
	struct allDrawings *f = in;
	
	// get gpio pointer
    unsigned int *gpioPtr = getGPIOPtr();
	
	//Our code goes here=======================================================
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
		
		//prompt user for input
		//printf("\nPlease press a button...\n");
		
		//while a button is not pressed
		while (!read_SNES(read_array));
		
		//display the values of the buttons pressed
		movement(read_array, f);
		
		//wait before prompting again
		wait(500000);
	}
	//=========================================================================
	
	pthread_exit(0);
}

void failed(char* id) {
	//printf("Failed to create thread %s \n", id);
}

void draw(struct drawing *toBeDrawed, short int *drawStructPtr) {
	int i=0;
	unsigned int quarter,byte,word;
	int height = toBeDrawed->height;
	int width = toBeDrawed->width;
	
	height = height + toBeDrawed->yOffset;
	width = width + toBeDrawed->xOffset;
	
	long int location;
	
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

void drawData(struct drawing *toBeDrawed, short int *drawStructPtr, int xOff, int yOff) {
	//TODO
	//Here I want to have the numbers be given a specific offset
	//and then call the actual draw function. This could allow for
	//modularity and save us some lines
}

void* drawThread(void * picture) {
		//printf("bruh");
		short int *car2Ptr=(short int *) vehicle2.pixel_data;
		short int *defaultBackPtr=(short int *) defaultBack.pixel_data;
		short int *challengePtr=(short int *) challenge.pixel_data;
		short int *frogPtr = (short int*) frog.pixel_data;
		short int *scorePtr = (short int*) score.pixel_data;
		short int *livesPtr = (short int*) lives.pixel_data;
		short int *movesPtr = (short int*) moves.pixel_data;
		
		//drawing the score label
		((struct allDrawings*)picture)->sd.height = 36;
		((struct allDrawings*)picture)->sd.width = 160;
		((struct allDrawings*)picture)->sd.xOffset = 864;
		((struct allDrawings*)picture)->sd.yOffset = 0;
		
		//drawing lives label
		((struct allDrawings*)picture)->ld.height = 36;
		((struct allDrawings*)picture)->ld.width = 160;
		((struct allDrawings*)picture)->ld.xOffset =864;
		((struct allDrawings*)picture)->ld.yOffset = 72;
		
		//drawing the moves label
		((struct allDrawings*)picture)->md.height = 36;
		((struct allDrawings*)picture)->md.width = 160;
		((struct allDrawings*)picture)->md.xOffset = 864;
		((struct allDrawings*)picture)->md.yOffset = 144;
		
		//drawing the gray background
		((struct allDrawings*)picture)->bg.height = 768;
		((struct allDrawings*)picture)->bg.width = 1024;
		((struct allDrawings*)picture)->bg.xOffset = 0;
		((struct allDrawings*)picture)->bg.yOffset = 0;
		
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
		
		//We need to move these to inside a loop or else drawing the numbers
		//is going to be weird
		draw(&(((struct allDrawings*)picture)->bg), defaultBackPtr);
		draw(&(((struct allDrawings*)picture)->sd), scorePtr);
		draw(&(((struct allDrawings*)picture)->ld), livesPtr);
		draw(&(((struct allDrawings*)picture)->md), movesPtr);
		
		int a;
		//for the first row of cars
		for (a = 0; a < 5; a++) {
			//for the first row of cars
			((struct allDrawings*)picture)->firstCars.carsRow[a].height = 36;
			((struct allDrawings*)picture)->firstCars.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->firstCars.carsRow[a].yOffset = 648;
			
			//for the third row of cars
			((struct allDrawings*)picture)->thirdCars.carsRow[a].height = 36;
			((struct allDrawings*)picture)->thirdCars.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->thirdCars.carsRow[a].yOffset = 576;
			
			//for the logs in the 6th row
			((struct allDrawings*)picture)->row6logs.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row6logs.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row6logs.carsRow[a].yOffset = 468;
			
			//for the logs in the 8th row
			((struct allDrawings*)picture)->row8logs.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row8logs.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row8logs.carsRow[a].yOffset = 396;
			
			//for the cars in the 11th row
			((struct allDrawings*)picture)->row11cars.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row11cars.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row11cars.carsRow[a].yOffset = 288;
			
			//for the cars in the 13th row
			((struct allDrawings*)picture)->row13cars.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row13cars.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row13cars.carsRow[a].yOffset = 216;
			
			//for the logs in the 16th row
			((struct allDrawings*)picture)->row16logs.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row16logs.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row16logs.carsRow[a].yOffset = 108;
			
			//for the logs in the 18th row
			((struct allDrawings*)picture)->row18logs.carsRow[a].height = 36;
			((struct allDrawings*)picture)->row18logs.carsRow[a].width = 72;		
			((struct allDrawings*)picture)->row18logs.carsRow[a].yOffset = 36;
			
		} 
		
		//first row of cars
		((struct allDrawings*)picture)->firstCars.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->firstCars.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->firstCars.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->firstCars.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->firstCars.carsRow[4].xOffset = 1764;
		
		//third row of cars
		((struct allDrawings*)picture)->thirdCars.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->thirdCars.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->thirdCars.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->thirdCars.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->thirdCars.carsRow[4].xOffset = 1764;
		
		//6th row of logs
		((struct allDrawings*)picture)->row6logs.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row6logs.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row6logs.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row6logs.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row6logs.carsRow[4].xOffset = 1764;
		
		//8th row of logs
		((struct allDrawings*)picture)->row8logs.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row8logs.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row8logs.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row8logs.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row8logs.carsRow[4].xOffset = 1764;
		
		//11th row of cars
		((struct allDrawings*)picture)->row11cars.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row11cars.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row11cars.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row11cars.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row11cars.carsRow[4].xOffset = 1764;
		
		//13th row of cars
		((struct allDrawings*)picture)->row13cars.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row13cars.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row13cars.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row13cars.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row13cars.carsRow[4].xOffset = 1764;
		
		//16th row of logs
		((struct allDrawings*)picture)->row16logs.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row16logs.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row16logs.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row16logs.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row16logs.carsRow[4].xOffset = 1764;
		
		//18th row of logs
		((struct allDrawings*)picture)->row18logs.carsRow[0].xOffset = 936;
		((struct allDrawings*)picture)->row18logs.carsRow[1].xOffset = 1080;
		((struct allDrawings*)picture)->row18logs.carsRow[2].xOffset = 1260;
		((struct allDrawings*)picture)->row18logs.carsRow[3].xOffset = 1476;
		((struct allDrawings*)picture)->row18logs.carsRow[4].xOffset = 1764;
		
		//draw(&(((struct allDrawings*)picture)->cg), challengePtr);
		
		while (1) {
			while(pauseFlag);
			
			//Draws base layer
			//draw(&(((struct allDrawings*)picture)->bg), defaultBackPtr);
			draw(&(((struct allDrawings*)picture)->cg), challengePtr);
			draw(&(((struct allDrawings*)picture)->f), frogPtr);
			
			for (a = 0; a < 5; a++) {
				
				
				//18th row of logs
				((struct allDrawings*)picture)->row18logs.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row18logs.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row18logs.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row18logs.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row18logs.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row18logs.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row18logs.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row18logs.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row18logs.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row18logs.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row18logs.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row18logs.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row18logs.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row18logs.carsRow[4].xOffset = 1764;
				}
				
				
				
				//16th row of logs
				((struct allDrawings*)picture)->row16logs.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row16logs.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row16logs.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row16logs.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row16logs.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row16logs.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row16logs.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row16logs.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row16logs.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row16logs.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row16logs.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row16logs.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row16logs.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row16logs.carsRow[4].xOffset = 1764;
				}
				
				
				
				//13th row of cars
				((struct allDrawings*)picture)->row13cars.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row13cars.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row13cars.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row13cars.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row13cars.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row13cars.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row13cars.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row13cars.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row13cars.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row13cars.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row13cars.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row13cars.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row13cars.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row13cars.carsRow[4].xOffset = 1764;
				}
				
				//11th row of cars
				((struct allDrawings*)picture)->row11cars.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row11cars.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row11cars.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row11cars.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row11cars.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row11cars.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row11cars.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row11cars.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row11cars.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row11cars.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row11cars.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row11cars.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row11cars.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row11cars.carsRow[4].xOffset = 1764;
				}
				
				
				//8th row of logs
				((struct allDrawings*)picture)->row8logs.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row8logs.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row8logs.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row8logs.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row8logs.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row8logs.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row8logs.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row8logs.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row8logs.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row8logs.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row8logs.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row8logs.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row8logs.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row8logs.carsRow[4].xOffset = 1764;
				}
				
				//6th row of logs
				((struct allDrawings*)picture)->row6logs.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->row6logs.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->row6logs.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->row6logs.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->row6logs.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->row6logs.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->row6logs.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->row6logs.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->row6logs.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->row6logs.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->row6logs.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->row6logs.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->row6logs.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->row6logs.carsRow[4].xOffset = 1764;
				}
				
				//third row
				((struct allDrawings*)picture)->thirdCars.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->thirdCars.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->thirdCars.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->thirdCars.carsRow[a]), car2Ptr);
				}
				
				
				if (((struct allDrawings*)picture)->thirdCars.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->thirdCars.carsRow[0].xOffset = 936;
				}
				
		
				if (((struct allDrawings*)picture)->thirdCars.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->thirdCars.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->thirdCars.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->thirdCars.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->thirdCars.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->thirdCars.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->thirdCars.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->thirdCars.carsRow[4].xOffset = 1764;
				}
				
				//first row
				((struct allDrawings*)picture)->firstCars.carsRow[a].xOffset -= 2;
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[a].xOffset <= 792 &&
				((struct allDrawings*)picture)->firstCars.carsRow[a].xOffset > 0) {
					draw(&(((struct allDrawings*)picture)->firstCars.carsRow[a]), car2Ptr);
				}
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[0].xOffset <= -540) {
					((struct allDrawings*)picture)->firstCars.carsRow[0].xOffset = 936;
				}
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[1].xOffset <= -396) {
					((struct allDrawings*)picture)->firstCars.carsRow[1].xOffset = 1080;
				}
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[2].xOffset <= -216) {
					((struct allDrawings*)picture)->firstCars.carsRow[2].xOffset = 1260;
				}
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[3].xOffset <= 0) {
					((struct allDrawings*)picture)->firstCars.carsRow[3].xOffset = 1476;
				}
				
				if (((struct allDrawings*)picture)->firstCars.carsRow[4].xOffset <= 288) {
					((struct allDrawings*)picture)->firstCars.carsRow[4].xOffset = 1764;
				}
				
			}
			
			//sleep(1);
			delayMicroseconds(1000);
			//delay(125);
		}
		
		
		
	
		pthread_exit(0);
}

/* main function */
int main(){

	/* initialize + get FBS */
	framebufferstruct = initFbInfo();
	
	struct allDrawings *picture = (struct allDrawings *)malloc(sizeof(struct allDrawings));
	
		
    pthread_t drawT;
	pthread_t playerMove;
	
	//printf("bruh\n");
    //launch the main thread
    pthread_create(&drawT, NULL, drawThread, picture);
    pthread_create(&playerMove, NULL, controllerMain, picture);

	while(1) {
		//This just runs and checks for car collisions
		checkCarCollision(picture);
	}

    //join back the main game thread once it cancels itself.
	pthread_join(drawT, NULL);
	pthread_join(playerMove, NULL);
	
	
	
	
	/* free pixel's allocated memory */
	free(picture);
	picture = NULL;
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	
	return 0;
}


