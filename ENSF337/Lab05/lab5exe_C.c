//  ENSF 337 - Fall 2020 Lab 5 Exercise C
//  M. Moussavi
//  lab5exe_C.c

#include "lab5exe_C.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(void) {
    Bits_Pattern bits_array[MAX_ARRAY_SIZE];
    int size = read_bit_patterns(bits_array,"lab5exe_C.txt");
    display_bit_patterns(bits_array, size);
    fprintf (stdout, "\nProgram terminated. \n");
    return 0;
}

int read_bit_patterns(Bits_Pattern * arr, const char* filename){
    
    // in you AR diagram you don't need to show where FILE pointer, fp, points to.
    // just show the arrow points to a box with a couple of question marks,
    // anywhere besid your diagram.
    
    FILE *fp;
    int row = 0, col;
    
    fp = fopen(filename, "r");
    
    if(fp == NULL){
        fprintf( stderr, "Error: cannot open file:  %s", filename);
        exit(1);
    }
    
    char c = fgetc(fp);
    
    while (c != EOF && row < MAX_ARRAY_SIZE){
        col = 0;
        while(c != '\n' && c != EOF){
            if(c != '0' && c != '1')
            {
                fprintf(stderr, "Error: Invlide character in: %s", filename);
                exit(1);
            }
            arr[row].bits[col] = c;
            col++;
            c = fgetc(fp);
        }
    
        arr[row].bits[col] ='\0';
        calculate_value(&arr[row]);
        row++;
        if(c != EOF)
            c = fgetc(fp);
    }
    fclose (fp);
    return row;
}

void calculate_value(Bits_Pattern * bits){
    
    int position = 0;
    bits->value =0;
    
    for ( int i = (int) strlen(bits->bits)-1; i >= 0; i-- ) {
        bits ->value = bits->value + (bits->bits[i]-48) * pow(2.00, position);
        position++;
    }
    // POINT ONE - DRAW AR DIAGRAM AT THIS POINT WHEN REACHES FOR THE SECOND TIME
    
}

void display_bit_patterns(Bits_Pattern arr[], int size){
    for(int i = 0; i < size; i++) {
        for( int j = 0; arr[i].bits[j] != '\0'; j++)
            fprintf(stdout, "%c", arr[i].bits[j]);
        fprintf(stdout, "       %12d\n", arr[i].value);
    }
}
