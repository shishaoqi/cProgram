/*
 * File: bigfact.c
 * ---------------
 * This file defines the BigFact function, which calculates
 * the factorial of an integer but returns the result as a
 * bigIntADT so that the result can be arbitrarily large.
 * This file also includes a test program that prints the
 * factorials of the numbers between the limits LowerLimit
 * and UpperLimit, inclusive.
 */

#include <stdio.h>
#include "genlib.h"
#include "bigint.h"

/* Constants */

#define LowerLimit  0
#define UpperLimit 20

/* Private function prototypes */

static bigIntADT BigFact(int n);

/* Main program */

main()
{
    int i;

    for (i = LowerLimit; i <= UpperLimit; i++) {
        printf("%2d! = %s\n", i, BigIntToString(BigFact(i)));
    }
}

/*
 * Function: BigFact
 * Usage: b = BigFact(n);
 * ----------------------
 * This function returns the factorial of n, which is defined
 * as the product of all integers from 1 up to n.
 */

static bigIntADT BigFact(int n)
{
    if (n == 0) {
        return (NewBigInt(1));
    } else {
        return (MultiplyBigInt(NewBigInt(n), BigFact(n - 1)));
    }
}
