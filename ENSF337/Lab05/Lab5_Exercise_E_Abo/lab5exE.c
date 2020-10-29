/* File: lab5exE.c
 * ENSF Fall 2020 - lab 5 - Exercise E
 */

#include "lab5exE.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(void)
{
    Point alpha = { "A1", 2.3, 4.5, 56.0} ;
    Point beta = { "B1", 25.9, 30.0, 97.0} ;
    printf ("Display the values in alpha, and beta: ");
    display_struct_point(alpha);
    display_struct_point(beta);
    
    Point *stp = &alpha;
    printf ("\n\nDisplay the values in *stp: ");
    display_struct_point(*stp);
    
    Point gamma = mid_point(stp, &beta, "M1");
    printf ("\n\nDisplay the values in gamma after calling mid_point function.");
    printf ("Expected result is: M1 <14.10, 17.25, 76.50>");
    
    printf("\n\nThe actual result of calling mid_point function is: ");
    display_struct_point(gamma);
    
    swap (stp, &beta);
    printf ("\n\nDisplay the values in *stp, and beta after calling swap function.");
    printf ("Expected to be:\nB1 <25.90, 30.00, 97.00>\nA1 <2.30, 4.50, 56.00>");
    
    
    printf("\n\nThe actual result of calling swap function is: ");
    display_struct_point(*stp);
    display_struct_point(beta);
    
    
    printf("\n\nThe distance between alpha and beta is: %.2f. ", distance(&alpha, &beta));
    printf ("(Expected to be: 53.74)");
    printf("\nThe distance between gamma and beta is: %.2f. ", distance(&gamma, &beta));
    printf ("(Expected to be: 26.87)");
    return 0;
}

void display_struct_point(const Point x)
{
    printf("\n%s <%.2lf, %.2lf, %.2lf>", x.label, x.x, x.y, x.z);
}


Point mid_point(const Point* p1, const Point* p2, const char* label)
{
    // This function is complete and was completed by the student
    // YOU ARE NOT ALLOWED TO USE ANY STRING LIBRARY FUNCTIONS IN THIS FUNCTION
	double d1, d2, d3;
	int i;
    Point middle = {"", 0, 0, 0};
	//Finds the differences
	d1 = (*p1).x + (*p2).x;
	d2 = (*p1).y + (*p2).y;
	d3 = (*p1).z + (*p2).z;
	//Finds the middle of the differences
	d1 /= 2; d2 /= 2; d3 /= 2;
	//Gives middle the correct label
	for (i = 0; i < 10; i++)
		middle.label[i] = label[i];
	//Gives middle the points
	middle.x = d1;
	middle.y = d2;
	middle.z = d3;
	
    return middle;
}

void swap(Point* p1, Point *p2)
{
    // This function is incomplete and must be completed by the students
	double d1Temp, d2Temp, d3Temp;
	char temp[10];
	int i;
	
	//Swaps labels
	for (i = 0; i < 10; i++) {
		temp[i] = (*p1).label[i];
		(*p1).label[i] = (*p2).label[i];
		(*p2).label[i] = temp[i];
	}
	
	//Swaps points
	d1Temp = (*p1).x; d2Temp = (*p1).y; d3Temp = (*p1).z;
	(*p1).x = (*p2).x; (*p1).y = (*p2).y; (*p1).z = (*p2).z;
	(*p2).x = d1Temp; (*p2).y = d2Temp; (*p2).z = d3Temp;
}

double distance(const Point* p1, const Point* p2)
{
    // This function is complete and was completed by the student
    // NOTE: IN THIS FUNCTION YOU ARE NOT ALLOWED TO USE THE ARROW OPERATOR ->
	
	//Differences
	double d1, d2, d3;
	//Total distance
	double dt;
	//Finds the differences
	d1 = (*p1).x - (*p2).x;
	d2 = (*p1).y - (*p2).y;
	d3 = (*p1).z - (*p2).z;
	//Squares the differences
	d1 *= d1; d2 *= d2; d3 *= d3;
	dt = sqrt(d1 + d2 + d3);
	
    return dt;
}