/*
 *
 * lab2exe_D1.c
 * ENSF 337 - Lab 2 - Execise D Part I
 *  This program lets you examine the behaviour of simple scanf calls
 *  in response to input errors.
 */

#include <stdio.h>

int main(void)
{
  int i = 333;
  double d = 1234.5;
  int nscan;

  printf("Please enter an integer: \n");
  nscan = scanf("%d", &i);
  printf("nscan is %d and i is %d\n", nscan, i);

  printf("Please enter a double: \n");
  nscan = scanf("%lf", &d);
  printf("nscan is %d and d is %f\n",  nscan, d);

  return 0;
}
