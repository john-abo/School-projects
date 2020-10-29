/*
 * Author: John Abo
 * UCID: 30088517
 * Date: September 25, 2020
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void display(int *table, int width, int length);
void topRelevantDocs(int *table, int searching, int topDocNum, int x, int y, FILE *log);
void farewell();
void sort(int *index, double *freq, int size);
int logToFile(int *table, int n, int m, int x, int y, FILE *log);
int initializeRandomTable(int *table, int x, int y); //Done
int initializeFromFile(int *table, char *argv[], int x, int y);
int checkCmdArg(int argc, char *argv[], int *x, int *y);
int randomNum(int m, int n);

int main(int argc, char *argv[]) {
	
	int i, j, width, length, input, fileExists = 0, searching = 1, topDoc = 1;
	char userInput, fileName[] = "assign1.log";
	FILE *outFile;
	srand(time(0));
	
	//Checks the validity of the commandline arguments
	fileExists = checkCmdArg(argc, argv, &width, &length);
	int table[width][length]; //Creates the array
	
	//Checks if there is a .log file that can be openned
	if ((outFile = fopen("assign1.log", "w")) == NULL) {
		printf("Unable to open the file: %s\n", argv[3]);
		fclose(outFile);
		farewell();
	}
	
	//Whether a file name is passed as a command line argument, will either:
	//Create a new table from the size given or copy the file's table to an array
	switch (fileExists) {
		case 1: initializeFromFile((int*)table, argv, width, length); break;
		case 0: initializeRandomTable((int*)table, width, length); break;
	}
	
	fprintf(outFile, ".log file \r\n");
	//Runs through the search functions of the program until the user enters 0 to exit
	while (topDoc) {
		display((int*)table, width, length);
		printf("\nEnter the index of the word you are searching for: ");
		scanf("%d", &searching);
		printf("How many top documents you want to retrieve? (Enter 0 to exit): ");
		scanf("%d", &topDoc);
		
		//If the user does not input an integer within the proper range, it will prompt the user again
		if (!(searching >= width || searching < 0) && !(topDoc > length || topDoc < 1)) {
			topRelevantDocs((int*)table, searching, topDoc, width, length, outFile);
			
			//Attempts to log to file, if an error occurs it will print an error message and end program
			if (logToFile((int*)table, searching, topDoc, width, length, outFile)) {
				printf("Unable to log to file");
				return 1;
			}
		} else if (topDoc != 0){
			printf("\n\tPlease enter valid integers\n");
		} else {
			farewell();
		}
	}
	
	fclose(outFile);
	return 0;
}

int randomNum(int max, int min) {
	int randomNumber;
	
	//Ensures the max is at least greater  than the min, then generates and returns a random number
	if (max > min) {
		randomNumber = (rand() % (max - min)) + min;
		return randomNumber;
	}
	
	return 0;
}

int checkCmdArg(int a, char *argv[], int *x, int *y) {
	//Any other value of a would imply the command line argument is invalid
	if (a == 3 || a == 4) {
		//Checks whether the size passed as command line argument is valid
		if ((atoi(argv[1]) > 20 || atoi(argv[1]) < 5) || (atoi(argv[2]) > 20 || atoi(argv[2]) < 5)) {
			printf("Please enter a valid size\n");
			farewell();
		}
		
		//Assigns the values of the arguments to x and y
		*x = atoi(argv[1]);
		*y = atoi(argv[2]);
		
		//If a table exists, the int values used to represent a boolean is changed
		if (a == 4) {
			return 1;
		}
			
		return 0;
	}
	printf("Please enter valid arguments\n");
	farewell();
	exit(1);
}

int initializeRandomTable(int *table, int x, int y) {
	
	int i,j;
	
	printf("\n\tCreating random table... \n");
	
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			*((table + i * y) + j) = randomNum(10, 0);
		}
	}
	
	return 0;
}

int initializeFromFile(int *table, char *argv[], int x, int y) {
	
	int i, j, fileNum;
	char fileChar, space;
	FILE *inFile;
	
	printf("\n\tInitializing form file... \n");
	
	if ((inFile = fopen(argv[3], "r")) == NULL) {
		printf("\nUnable to open the file: %s\n", argv[3]);
		fclose(inFile);
		farewell();
	}
	
	//Copy the value in the file to the table
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			fileChar = fgetc(inFile);
			fileNum = fileChar - 48;
			*((table + i * y) + j) = fileNum;
			
			//This ensures ensures any data that isn't an integer isn't written to the array
			//by going back to the same array element
			if ((fileChar == ' ' || fileChar == '\n') || (fileChar == '\r' || fileChar == '\f')) {
				j--;
			}
		}
	}
	
	fclose(inFile);
	return 0;
}

int logToFile(int *table, int searching, int topDoc, int x, int y, FILE *log) {
	
	int i, j, data;
	
	printf("\n\tLogging to file...\n");
	
	fprintf(log, "Table:\r\n");
	
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			data = *((table + i * y) + j);
			
			fprintf(log, "%d ", data);
		}
		fprintf(log, "\r\n");
	}
	
	return 0;
}

void display(int *table, int width, int length) {
	int i,j;
	
	printf("\nDisplaying table:\n");
	
	for (i = 0; i < width; i++) {
		for (j = 0; j < length; j++) {
			printf("%d ", *((table + i*length) + j));
		}
		printf("\n");
	}
}

void topRelevantDocs(int *table, int searching, int topDocNum, int x, int y, FILE *log) {
	
	int column[y], index[y];
	double frequency[y];
	int i, j;
	
	//Places columns into an array and makes sure the values in frequency are 0;
	for (i = 0; i < y; i++) {
		column[i] = *((table + i*y) + searching);
		frequency[i] = 0;
		index[i] = i;
	}
	
	//Finds the frequency for each value in the column
	for (i = 0; i < y; i++) {
		for (j = 0; j < y; j++) {
			frequency[i] += (*((table + i*y) + j));
		}
		frequency[i] = (column[i] / frequency[i]) * 100;
	}
	
	printf("\n\tSearching...\n");
	sort(index, frequency, y);
	printf("\nresults (From lowest to highest freqency):\n");
	
	//Prints the results to the user
	for (i = 0; i < topDocNum; i++) {
		printf("\nResult #%d: %d\n", i + 1, index[i]);
		fprintf(log, "\r\nResult #%d: %d\r\n", i + 1, index[i]);
	} 
	
	fprintf(log, "Column Searched: %d\r\n", searching);
	fprintf(log, "Number of top documents requested: %d\r\n", topDocNum);
}

void sort(int *index, double *freq, int size) {
	
	//This uses code similar to my implementation
	//of a selection sort from a previous project
	
	int i, indexOfHighest;
	
	printf("\n\tSorting...\n");
	
	for  (i = 0; i < size; i++) {
		
		//Temporary values that will be used to swap values
		int tempIn, j = 0;
		double tempFr;
		double high = 0;
		indexOfHighest = -1;
		
		//Looks for highest value and sets it to highest while recording the index
		for (j = i; j < size; j++) {
			if (high < freq[j]) {
				high = freq[j];
				indexOfHighest = j;
			}
		}
		
		//Swaps the values of the current index position and the highest values
		//found down the array
		if (index >= 0) {
			tempFr = freq[i]; tempIn = index[i];		
			freq[i] = freq[indexOfHighest]; index[i] = index[indexOfHighest];
			freq[indexOfHighest] = tempFr; index[indexOfHighest] = tempIn;
		}
	}
}

void farewell() {
	//Greets the user farewell before ending the program, will be used
	//to end the program, such as errors in file IO
	printf("\n\tFarewell\n");
	exit(1);
}