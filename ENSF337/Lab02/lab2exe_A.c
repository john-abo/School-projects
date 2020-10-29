/*
 * lab2exe_A.c
 * ENSF 337 - Lab 2 - Execise A
 * An exercise to help you understand the use of pointer variables.
 */

#include <stdio.h>

int main(void)
{
  int foo; 
  int bar;
  int *fred;
  int *sam;

  bar = 100;
  foo = 200;
  fred = &foo;

  /* point one */

  printf("point one: foo is %d and bar is %d.\n", foo, bar);
  sam = &bar;
  *sam += 30;
  *fred -= 40;

  /* point two */

  printf("point two: foo is %d and bar is %d.\n", foo, bar);
  fred = &bar;
  *fred += 5;

  /* point three */

  printf("point three: foo is %d and bar is %d.\n", foo, bar);
  printf("point three: *fred is %d and *sam is %d.\n", *fred, *sam);
  sam = &foo;
  *sam = *fred;

  /* point four */

  printf("point four: foo is %d and bar is %d.\n", foo, bar);
  *sam *= 100;
  sam = fred;

  /* point five */

  printf("point five: foo is %d and bar is %d.\n", foo, bar);
  printf("point five: *fred is %d and *sam is %d.\n", *fred, *sam);  

  return 0;
}
