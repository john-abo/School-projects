//  ENSF 33F - Fall 2020 Lab 5 Exercise C
//  M. Moussavi
//  lab5exe_C.h

#ifndef lab5ExC_h
#define lab5ExC_h

#define MAX_BITS 5
#define MAX_ARRAY_SIZE 4

typedef struct Bits_Pattern
{
    char bits[MAX_BITS];
    int value;
    
} Bits_Pattern;


int read_bit_patterns(Bits_Pattern * arr, const char* filename);

void display_bit_patterns(Bits_Pattern arr[], int size);

void calculate_value(Bits_Pattern * byte);

#endif /* lab5ExC_h */
