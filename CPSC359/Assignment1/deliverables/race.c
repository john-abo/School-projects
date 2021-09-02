/*
 * The program takes in 2 numbers as parameters, the number of horses,
 * and the finish line. It then creates n+1 threads, a main thread and n 
 * horses. The main thread checks for when all the horses reach the
 * finish line, and then ends the race. The horse threads sleep for a certain
 * amount of time, and goes to one step until the finish line. The program
 * then prints the result of the race.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *mainThread(void *param);
void *hThread(void *id);
int randomNum(int min, int max);
int argTest(int argc, char* argv[]);
void checkEnd(int n);
void printRace(int n);

//Struct that contains race data
typedef struct {
	int* progress;
	int* rank;
	int* finishedTime;
	int placement;
	int raceFinished;
	int start;
	int dist;
} shared;

//Struct is global
shared race;

int main(int argc, char* argv[]) {
	
	//Ensures the user as entered an approriate number
	if (argTest(argc, argv)) {
		exit(1);
	}
	
	//Creates variables
	int i;
	int n = atoi(argv[1]);
	pthread_t threads[n+1];
	
	//Sets up the race struct
	int prog[n];
	int rank[n];
	int finTime[n];
	int idArr[n];
	
	//Populates arrays that will be placed in the struct
	for (i = 0; i < n; i++) {
		prog[i] = 0;
		rank[i] = -1;
		finTime[i] = -1;
		idArr[i] = i;
	}
	
	//Assigns all the values to the struct
	race.progress = prog;
	race.rank = rank;
	race.finishedTime = finTime;
	race.placement = 1;
	race.raceFinished = 0;
	race.start = 0;
	race.dist = atoi(argv[2]);
	
	//Creates main thread
	if (pthread_create(&threads[0], NULL, mainThread, &n))
		printf("failed to create thread\n");
	
	//Creates horse threads
	for (i = 1; i <= n; i++) {
		if (pthread_create(&threads[i], NULL, hThread, &(idArr[i])))
			printf("failed to create thread\n");
	}
	
	//Starts the race, also ensures all horses are created before race starts
	sleep(1);
	printf("Ready?\n");
	sleep(1);
	printf("set?\n");
	sleep(1);
	printf("Go!\n");
	race.start = 1;
	
	//Waits until the race is finished before moving on
	while(!race.raceFinished);
	
	//Ends all the threads
	for (i = 0; i <= n; i++) {
		pthread_join(threads[i], NULL);
	}
	
	//Prints race results
	printf("Race stats: \n");
	for (i = 0; i < n; i++) {
		printf("Horse #%d", (i + 1));
		printf(", Time: %d", race.finishedTime[i]);
		printf(", Rank: %d\n", race.rank[i]);
	}
	
	return 0;
}

/*
 * Function run by main thread. checks for end of race and prints horse. The
 * input is the number of horses in the race.
 */
void *mainThread(void *param) {
	
	int n = *((int*)param);
	
	printf("%d horses and the finish line is %d spaces away\n", n, race.dist);
	
	//Waits for the race to start
	while(!race.start);
	
	//Prints race and checks horse positions
	while(!race.raceFinished) {
		sleep(1);
		checkEnd(n);
		printRace(n);
	}
	
	printf("\tRace finished\n");
	
	//Terminates thread
	pthread_exit(0);
}

/*
 * Function run by horse threads. Randomly generates a number between 0 and 3,
 * which is the time between incrementing the prosition of the horse. This
 * allows the horses to move at random times and simulate the race. The input
 * is the number assigned to the horse.
 */
void *hThread(void *identity) {
	
	int id = *((int*)identity);
	int startTime;
	
	srand(time(0));
	
	//Waits for start flag to be raised
	while(!race.start);
	
	startTime = time(0);
	
	//Goes on until the horse has finish or until the race ends
	//Note: The race will not end until all horses finish
	while(!race.raceFinished) {
		if (race.progress[id] <= race.dist) {
			sleep(randomNum(0,3));
			race.progress[id]++;
		} else {
			race.rank[id] = race.placement;
			race.placement++;
			race.finishedTime[id] = time(0) - startTime;
			break;
		}
	}
	
	printf("\tHorse %d has finished!\n", id);
	
	//Waits for race to end as the horse has finished
	while(!race.raceFinished);
	
	pthread_exit(0);
}

/*
 * Tests the arguments passed by the user. This ensures the number of horses is
 * less than or equal to 100 and makes sure the distance isn't too short. The
 * inputs are the command line arguments passed into main.
 */
int argTest(int argc, char* argv[]) {
	
	//Ensures there is the right number of arguments that can be checked
	if (argc == 3) {
		
		//Ensures number of horses is greater than 1
		if (atoi(argv[1]) < 1) {
			printf("Argument too small\n");
			return 1;
		}
		
		//Ensures number of horses is less than 101
		if (atoi(argv[1]) > 100) {
			printf("Argument too large\n");
			return 1;
		}
		
		//Ensures distance is greater than 0
		if (atoi(argv[2]) < 1) {
			printf("That distance is too short\n");
			return 1;
		}
		
		//Returns 0 if all arguments pass
		return 0;
	}
	
	//Not the right amount of arguments have been passed
	printf("Not the right amount of arguments\n");
	return 1;
}

/*
 * Random number generater recycled from past projects. Allows user to pass min
 * and max values that improves usablility of rand().
 */
int randomNum(int min, int max) {
	int randomNumber;
	
	//Ensures the max is at least greater  than the min
	//then generates and returns a random number
	if (max > min) {
		randomNumber = (rand() % (max - min)) + min;
		return randomNumber;
	}
	
	return 0;
}

/*
 * Checks if all the horses have finished the race, once they do, raise end
 * flag to end the race. It is passed the number of horses in the race.
 */
void checkEnd(int n) {
	int i;
	
	//Checks the ranks to see if there is still an unranked horse
	for (i = 0; i < n; i++) {
		if (race.rank[i] < 0) {
			return;
		}
	}
	
	race.raceFinished = 1;
}

/*
 * Prints the horses in their relative positions to the user. A > signifies
 * the horse is still running, while an * shows the race has finished. It is
 * passed the number of horses in the race.
 */
void printRace(int n) {
	int i, j;
	
	//This clears the console to allow for the illusion of a race
	//Borrowed from stackoverflow: 
	//https://stackoverflow.com/questions/17271576/clear-screen-in-c-and-c-on-unix-based-system
	printf("\033[H\033[J");
	
	//Prints the horses position before checking
	for (i = 0; i < n; i++) {
		printf("Horse #%d: \t", (i + 1));
		
		//Prints a '-' for each space the horse has travelled
		for (j = 0; j < race.progress[i]; j++) {
			printf("-");
		}
		
		//Determines which end to place depending on the horse's position
		if (race.progress[i] > race.dist) {
			printf("*\n");
		} else {
			printf("> %d\n", race.progress[i]);
		}
	}
}
