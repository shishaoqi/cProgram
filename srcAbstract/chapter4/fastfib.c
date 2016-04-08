/*
 * File: fastfib.c
 * ---------------
 * This program is identical in its operation to fib.c but
 * implements the Fibonacci function in a more efficient way.
 */

#include <stdio.h>
#include "genlib.h"

/*
 * Constants
 * ---------
 * MinIndex -- Index of first term to generate
 * MaxIndex -- Index of last term to generate
 */

#define MinIndex  0
#define MaxIndex 12

/* Private function prototypes */

static int Fib(int n);
static int AdditiveSequence(int n, int t0, int t1);

/* Main program */

main()
{
    int i;

    printf("This program lists the Fibonacci sequence.\n");
    for (i = MinIndex; i <= MaxIndex; i++) {
        printf("Fib(%d)", i);
        if (i < 10) printf(" ");
        printf(" = %4d\n", Fib(i));
    }
}

/*
 * Function: Fib
 * Usage: t = Fib(n);
 * ------------------
 * This function returns the nth term in the Fibonacci
 * sequence.  In this implementation, the function Fib is
 * a simple wrapper that supplies additional arguments to
 * the more general function AdditiveSequence, which
 * does all the actual work.
 */

static int Fib(int n)
{
    return (AdditiveSequence(n, 0, 1));
}

/*
 * Function: AdditiveSequence
 * Usage: t = AdditiveSequence(n, t0, t1);
 * ---------------------------------------
 * This function returns the nth term in any additive
 * sequence in which the first two terms are t0 and t1
 * and all subsequent  terms are the sum of the preceding
 * two terms.  The recursive  insight is that the nth term
 * in such a series is the (n-1)st term in the series that
 * begins with t1 and t0 + t1.
 */

static int AdditiveSequence(int n, int t0, int t1)
{
    if (n == 0) return (t0);
    if (n == 1) return (t1);
    return (AdditiveSequence(n - 1, t1, t0 + t1));
}

