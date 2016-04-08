/*
 * File: oddeven.c
 * ---------------
 * This file defines mutually recursive IsOdd and IsEven
 * functions.
 */

#include <stdio.h>
#include <string.h>
#include "genlib.h"
#include "simpio.h"

/*
 * Constants
 * ---------
 * LowerLimit -- Starting value for odd/even table
 * UpperLimit -- Final value for odd/even table
 */

#define LowerLimit  0
#define UpperLimit 10

/* Function prototypes */

static bool IsEven(unsigned n);
static bool IsOdd(unsigned n);

/* Main program */

main()
{
    unsigned i;

    for (i = LowerLimit; i <= UpperLimit; i++) {
        if (IsEven(i)) {
            printf("%2d is even.\n", i);
        } else {
            printf("%2d is odd.\n", i);
        }
    }
}

/*
 * Function: IsEven
 * Usage: if (IsEven(n)) . . .
 * ---------------------------
 * This function returns TRUE if n is even.  The number 0
 * is considered even by definition; any other number is
 * even if its predecessor is odd.  Note that this function
 * is defined to take an unsigned argument and is therefore
 * not applicable to negative integers.
 */

static bool IsEven(unsigned n)
{
    if (n == 0) {
        return (TRUE);
    } else {
        return (IsOdd(n - 1));
    }
}

/*
 * Function: IsOdd
 * Usage: if (IsOdd(n)) . . .
 * --------------------------
 * This function returns TRUE if n is odd, where a number
 * is defined to be odd if it is not even.  Note that this
 * function is defined to take an unsigned argument and is
 * therefore not applicable to negative integers.
 */

static bool IsOdd(unsigned n)
{
    return (!IsEven(n));
}
