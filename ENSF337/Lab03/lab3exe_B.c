/* lab3exe_B.c
 * ENSF 337, Lab 3 Exercise B
 */

#include <stdio.h>
#include <string.h>

int foo(const char *s1, int ch, char* s2);
void bar(const char *x, char *y, char c, int n, int *m);

int main(void)
{
    const char* str1 = "Banana Bread";
    char str2[7];
    unsigned long int n1 = strlen(str1);
    int n2 = sizeof(str1);
    
    printf("str1 is %d bytes, and its length is %lu\n", n2, n1);
    int c = 'a';
    foo(str1, c, str2);
    
    return 0; 
}

int foo(const char *s1, int ch, char* s2)
{
    int i, j;
    for(i = 0, j=0; s1[i]; i++) {
        bar(s1, s2, ch, i, &j );
        
    }
	printf("i %d, j %d\n", i, j);
    // point two
	
	for (i = 0; s2[i]; i++)
		printf("%c ", s2[i]);
    return 0;
}

void bar(const char *x, char *y, char c, int n, int *m) {
    if(x[n] != c) {
        y[*m] = x[n];
        (*m)++;
		printf("BAR called, at i: %d and j: %d and y[*m]: %c\n", n, *m, y[*(m-1)]);
        // point one - when program reaches this point for the first time
    }
}
