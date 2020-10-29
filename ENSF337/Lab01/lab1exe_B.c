/*
* File Name: lab1exe_B.c
* Assignment: Lab 1 Exercise B
* Lab Section: B01
* Completed By: John Abo
* Submission Date: Sept 18, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 9.8   /* G is a constant: gravitation acceleration 9.8 m/s^2 */
#define PI 3.141592654  /* PI is constant */

void create_table(double v);
double Projectile_travel_time(double a, double v);
double Projectile_travel_distance(double a, double v);
double degree_to_radian(double d);

int main(void)
{
    int n;
    double velocity;
    
    printf ("please enter the velocity at which the projectile is launched (m/sec): ");
    n = scanf("%lf" ,&velocity);
    if(n != 1)
    {
        printf("Invlid input. Bye...");
        exit(1);
    }
    
    while (velocity < 0 )
    {
        printf ("please enter a positive number for velocity: ");
        n = scanf("%lf", &velocity);
        if(n != 1)
        {
            printf("Invlid input. Bye...");
            exit(1);
        }
    }
    
		create_table(velocity);
    
    return 0;
}

void create_table(double a) {
	double i;
	
	printf("Angle \t\t t \t\t d \n");
	printf("-----------------------------------------\n");
	for (i = 0; i < 90; i += 5) {
		printf("%lf \t %lf \t %lf \n", i, Projectile_travel_time(i, a), Projectile_travel_distance(i, a));
	}
}

double Projectile_travel_time(double a, double v) {
	double time;
	
	time = 2 * v * sin(degree_to_radian(a)) / G;
	
	return time;
}

double Projectile_travel_distance(double a, double v) {
	double distance;
	
	distance = (v * v) * sin(2 * degree_to_radian(a)) / G;
	
	return distance;
}

double degree_to_radian(double d) {
	double rad;
	
	rad = (d * PI) / 180;
	
	return rad;
}

/* UNCOMMENT THE CALL TO THE create_table IN THE main FUNCTION, AND COMPLETE THE PROGRAM */

