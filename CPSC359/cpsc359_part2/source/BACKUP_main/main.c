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
#include <unistd.h>
#include <wiringPi.h>
#include "initGPIO.h"

//Defines the pin nunber for CLK, DAT, and LAT
#define LAT 9
#define DAT 10
#define CLK 11

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

struct fbs framebufferstruct;
void drawPixel(void *pixel);
void *controllerMain(void *in);

int Read_Data();
void init_GPIO(unsigned int *GPIO_PTR);
void Write_Latch(int write);
void Write_Clock(int write);
void wait(int mill);
void movement(int* read_array);
int read_SNES(int* read_array);

void failed(char* id);

//declaring global variables
static unsigned *gpio;		//pointer storing the memory address of the base address of the GPIO Pins
static char end = 0;
static char* buttons[12] = {"B", "Y", "Select", "Start", "Up", "Down", "Left", "Right", "A", "X", "L", "R"};	//char array that holds all of the types of buttons in the sequence they are read

int controllerInput = 0;

struct drawing *f;
struct drawing *bg;
struct drawing *cg;

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
		default: printf("how'd you get here \n"); break;
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
		default: printf("how'd you get here \n"); break;
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
void movement(int* read_array) {
	//declaring local variables
	int i;
	
	//for all the buttons in the array
	for (i = 0; i < 12; i++) {
		
		//if it is pressed, display a message to the console
		if (!read_array[i]) {
			printf("\nYou have pressed %s \n", buttons[i]);
			
			//f->xOffset = 828;
			//f->yOffset = 684;
			
			switch (i) {
			case 4: f -> yOffset -= 36; break; //Up
			case 5: f -> yOffset += 36; break; //Down
			case 6: f -> xOffset -= 36; break; //Left
			case 7: f -> xOffset += 36; break; //Right
		}
		}
		
		//if the start button is pressed set the flag end to 1 so that the program will end
		if (!read_array[3]) {
			end = 1;
		}
	}
}

void draw(struct drawing *toBeDrawed, short int *drawStructPtr) {
	int i=0;
	unsigned int quarter,byte,word;
	int height = toBeDrawed->height;
	int width = toBeDrawed->width;
	
	char *tempBuffer = malloc(1024 * 768 * 2 + 1);
	
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
				
				printf("Bruh\n");
				
				*((unsigned short int*)(tempBuffer + location)) = toBeDrawed->color;
				i++;
				
		}
	}
	
	//memcpy(framebufferstruct.fptr, tempBuffer, 1024 * 768 * 2 + 1);
}

void *controllerMain(void *in)
{
	
	char* name = in;
	
	// get gpio pointer
    unsigned int *gpioPtr = getGPIOPtr();
	
	//Our code goes here=======================================================
	//Plenty of the code will be borrowed from the files given to us by the
	//prof and TAs
	
	//array that stores the values read from the controller
	int read_array[16];
	
	gpio = gpioPtr;
	
	printf("Created by: John Abo and Hilton Luu\n");
	printf("pointer address: %p\nThread ID %s \n", gpioPtr, name);
	
	gpio[7] = 0;
	gpio[10] = 0;
	gpio[13] = 0;
	
	//initializing the GPIO Pins
	init_GPIO(gpioPtr);
	
	//while the start button is not being pressed
	while (!end) {
		
		//prompt user for input
		printf("\nPlease press a button...\n");
		
		//while a button is not pressed
		while (!read_SNES(read_array));
		
		//display the values of the buttons pressed
		movement(read_array);
		
		//wait before prompting again
		wait(500000);
	}
	//=========================================================================
	
	pthread_exit(0);
}

void failed(char* id) {
	printf("Failed to create thread %s \n", id);
}

/* main function */
int main(){

	/* initialize + get FBS */
	framebufferstruct = initFbInfo();
	
	f = malloc(sizeof(struct drawing));
	bg = malloc(sizeof(struct drawing));
	cg = malloc(sizeof(struct drawing));
	
	short int *defaultBackPtr=(short int *) defaultBack.pixel_data;

	bg->height = 768;
	bg->width = 1024;
	bg->xOffset = 0;
	bg->yOffset = 0;
	
	short int *challengePtr=(short int *) challenge.pixel_data;
	
	cg->height = 720;
	cg->width = 864;
	cg->xOffset = 0;
	cg->yOffset = 0;
	
	short int *froggerPtr=(short int *) forg.pixel_data;
	
	f->height = 36;
	f->width = 36;
	f->xOffset = 828;
	f->yOffset = 684;
	
	pthread_t threads[26];
	
	//Creating threads and verifying their creation
	if (pthread_create(&threads[0], NULL, controllerMain, "player"))
		failed("player");
	
	//int i;
	
	while (!end) {
		draw(bg, defaultBackPtr);
		draw(cg, challengePtr);
		draw(f, froggerPtr);
	}
	
	
	/* free pixel's allocated memory */
	free(bg);
	free(f);
	bg = NULL;
	f = NULL;
	munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
	
	return 0;
}





