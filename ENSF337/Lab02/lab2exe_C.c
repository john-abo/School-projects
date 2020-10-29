/*
 *
 * lab2exe_C.c
 * ENSF 337 - Lab 2 - Execise C
 */

#include <stdio.h>
#include <stdlib.h>

void time_convert(int ms_time, int *minutes_ptr, double *seconds_ptr);
/* 
 * Converts time in milliseconds to time in minutes and seconds.
 * For example, converts 123400 ms to 2 minutes and 3.4 seconds.
 * REQUIRES
 *    ms_time >= 0.
 *    minutes_ptr and seconds_ptr point to variables.
 * PROMISES
 *    0 <= *seconds_ptr & *seconds_ptr < 60.0
 *    *minutes_ptr minutes + *seconds_ptr seconds is equivalent to
 *    ms_time ms.
 */ 

int main(void)
{
  int millisec;
  int minutes;
  double seconds;   
  int nscan;

  printf("Enter a time interval as an integer number of milliseconds: ");
  nscan = scanf("%d", &millisec);
  
  if (nscan != 1) {
    printf("Unable to convert your input to an int.\n");
    exit(1);
  }
  
  printf("Doing conversion for input of %d ms ... \n", millisec);

  /* MAKE A CALL TO time_convert HERE. */
  time_convert(millisec, &minutes, &seconds);

  printf("That is equivalent to %d minute(s) and %f second(s).\n", minutes,
	 seconds);

  return 0;
}

/* WRITE YOUR FUNCTION DEFINITION FOR time_convert HERE. */

/*
 * Converts milliseconds to the equivalent time in Minutes and Seconds and assignes those values to minutes_ptr
  * and seconds_ptr
 * REQUIRES: x >= 0
 * PROMISES: Seconds (0 <= seconds_ptr < 60) and Minutes equivelent to provided ms_time
 */

 void time_convert(int ms_time, int *minutes_ptr, double *seconds_ptr) {
	 *minutes_ptr = ms_time / (1000 * 60);//some math
	 *seconds_ptr = (ms_time / 1000.0) - (*minutes_ptr * 60);//Some more math
 }
