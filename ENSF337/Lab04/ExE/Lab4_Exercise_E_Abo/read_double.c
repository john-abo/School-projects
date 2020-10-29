int read_double(char*, int, double*);
int get_string(char*, int);
int is_valid_double(const char*);
double convert_to_double(const char*);

#include <string.h>
#include <math.h>

int read_double(char* digits, int size, double* num) {
	
	if (get_string(digits, size) == EOF)
			return EOF;
	
	if(is_valid_double(digits)){
		if(digits[0] == '-')
			*num = -convert_to_double(digits + 1);
		else if(digits[0] == '+') 
			*num = convert_to_double(digits + 1);
		else
			*num = convert_to_double(digits);
		return 1;
	}
	
	return 0;
}

double convert_to_double(const char* digits) {
	double sum = 0;
	int i = 0, j = 1, dec = 0;
	while (digits[i] != '\0') {
		
		if (digits[i] == '.') {
			dec = 1;
			i++;
		}
		
		if (dec == 1) {
			sum = sum + ((digits[i] - '0') / pow(10.0, j));
			j++;
		} else {
			sum = 10 * sum + (digits[i] - '0');
		}
		
		i++;
	}
	
	return sum;
}

int is_valid_double(const char* digits) {
	int valid = 1;
	int i, dec = 0;
	
	if (digits[0] == '+' || digits[0] == '-')
		i = 1;
	else
		i = 0;
	
	if (digits[i] == '\0')
		valid = 0;
	else {
		while (valid && (digits[i] != '\0')) {
			
			//Increases decimal count by 1
			if (digits[i] == '.')
				dec++;
			
			//Checks if it is valid
			if(digits[i] < '0' ||  digits[i] > '9') {
				if  (digits[i] != '.') {
					valid = 0;
				}
				if (dec > 1) {
					valid = 0;
				}
			}
			
			i++;
		}
	}
	
	return valid;
}

int get_string (char* digits, int size) {
	int i = 0, j = 0;
	int length;
	
	if (fgets(digits, size, stdin) == NULL)
		return EOF;
	
	length = strlen(digits);
	
	if (digits[length - 1] == '\n') {
		digits[length - 1] == '\0';
		length--;
	}
	
	while (digits[i] == ' ')
		i++;
	
	length -= i;
	
	while (j <= length) {
		digits[j] = digits[i];
		i++;
		j++;
	}
	
	if (length > 0) {
		j = length - 1;
		
		while (digits[j] == ' ')
			j--;
		
		length = j + 1;
		digits[length] = '\0';
	}
	
	return 1;
}