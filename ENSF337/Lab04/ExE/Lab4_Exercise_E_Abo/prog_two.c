#include <stdio.h>
#include <limits.h>

#include "read_double.c"

#define SIZE 50

int main() {
	
	double n = 0;
	char digits[SIZE];
	
	int y = EOF;
	
	while (1) {
		
		printf("\nEnter a real number, or press Ctrl + D to exit: ");
		
		y = read_double(digits, SIZE, &n);
		
		if (y == 1) 
			printf("\nYour number: %lf\n", n);
		else if (y == EOF) {
			printf("\n\tFarewell\n");
			break;
		}
		else
			printf("\n%s is an invalid number", digits);
		
	}
	
	return 0;
}