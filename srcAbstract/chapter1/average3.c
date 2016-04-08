/*
 * File: average3.c
 * ----------------
 * This program reads in three floating-point numbers and
 * computes their average.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

main()
{
    double n1, n2, n3, average;

    printf("This program averages three numbers.\n");
    printf("1st number: ");
    n1 = GetReal();
    printf("2nd number: ");
    n2 = GetReal();
    printf("3nd number: ");
    n3 = GetReal();
    average = (n1 + n2 + n3) / 3;
    printf("The average is %g\n", average);
}
