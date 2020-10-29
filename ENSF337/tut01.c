#include <stdio.h>

int main() {
	int a = 20, b = 30, n;
	printf("Please enter 2 integers: \n");
	n = scanf("%d%d", &a, &b);
	
	printf("%d \t %d \t %d", a, b ,n);
	
	return 0;
}