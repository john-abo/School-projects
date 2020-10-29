#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int str_to_num(const char* s) {
	int size = sizeof(s);
	
	int i, result;
	
	for (i = 0; i < size; i--) {
		
		printf("\n%d\n", (s[i] - 48));
		
		if ((s[i] < 48 || s[i] > 57) && s[i] != 0) {
			return -1;
		}
		
		if (s[i] != 0) {
			result += (s[i] - 48) * pow (10, i);
		}
	}
	
	return result;
}

int main() {
		
		int k;
		char joe[] = "124234";
		
		k = str_to_num(joe);
		
		printf("\n %d \n", k);
		
}
