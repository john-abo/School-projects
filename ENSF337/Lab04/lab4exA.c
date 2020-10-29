// lab4exA.c

// ENSF 337 Fall 2020 Lab 4 Exercise A

#include <stdio.h>

int main(void)
{
    int i, j;
    int a[ ] = { 1000, 1200, 300, 1400, 1500, 30 };
    int *z;
    
    *a = 99;
    *(a + 2) = 55;
    a[1] = 11;
    
    z = a + 2;
    
    i = 2;
    
	//These 2 do the same thing
    *(z-1) = *(z-i) + z[0];
    z[1] = *z * i;
	
	//
    *(z+2) = *(&a[2] - i);
    z + 3;
    z[3] = -9;
    
    i = a - &a[6];
    // point one
    
	printf("\ni = %d\n", i);
	printf("\n*z = %d\n", *z);
	
	for (j = 0; j < 6; j++) {
		printf("\na[%d] = %d", j, a[j]);
	}
	
    return 0;
}

