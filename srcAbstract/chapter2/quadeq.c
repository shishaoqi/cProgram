/*
 * File: quadeq.c
 * --------------
 * This program finds roots of the quadratic equation
 *
 *         2
 *     a x    +  b x  +  c  =  0
 */

#include <stdio.h>
#include <math.h>
#include "genlib.h"
#include "simpio.h"

/* Private function prototypes */

static void GetCoefficients(double *pa, double *pb, double *pc);
static void SolveQuadratic(double a, double b, double c, double *px1, double *px2);
static void DisplayRoots(double x1, double x2);

/* Main program */

main()
{
    double a, b, c, x1, x2;

    GetCoefficients(&a, &b, &c);
    SolveQuadratic(a, b, c, &x1, &x2);
    DisplayRoots(x1, x2);
}

/*
 * Function: GetCoefficients
 * Usage: GetCoefficients(&a, &b, &c);
 * -----------------------------------
 * This function is responsible for reading in the coefficients
 * of a quadratic equation.  The values of the coefficients are
 * passed back to the main program in the variables a, b, and c,
 * which are passed by reference.
 */

static void GetCoefficients(double *pa, double *pb, double *pc)
{
    printf("Enter coefficients for the quadratic equation:\n");
    printf("a: ");
    *pa = GetReal();
    printf("b: ");
    *pb = GetReal();
    printf("c: ");
    *pc = GetReal();
}

/*
 * Function: SolveQuadratic
 * Usage: SolveQuadratic(a, b, c, &x1, &x2);
 * -----------------------------------------
 * This function solves a quadratic equation.  The coefficients
 * are supplied as the arguments a, b, and c, and the roots are
 * returned in x1 and x2, which are passed by reference.
 */

static void SolveQuadratic(double a, double b, double c, double *px1, double *px2)
{
    double disc, sqrtDisc;

    if (a == 0) Error("The coefficient a must be nonzero");
    disc = b * b - 4 * a * c;
    if (disc < 0) Error("The solutions are complex numbers");
    sqrtDisc = sqrt(disc);
    *px1 = (-b + sqrtDisc) / (2 * a);
    *px2 = (-b - sqrtDisc) / (2 * a);
}

/*
 * Function: DisplayRoots
 * Usage: DisplayRoots(x1, x2);
 * ----------------------------
 * This function displays the values x1 and x2, which are
 * the roots of a quadratic equation.
 */

static void DisplayRoots(double x1, double x2)
{
    if (x1 == x2) {
        printf("There is a double root at %g\n", x1);
    } else {
        printf("The roots are %g and %g\n", x1, x2);
    }
}
