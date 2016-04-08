/*
 * File: fib.c
 * -----------
 * This program lists the terms in the Fibonacci sequence with
 * indices ranging from MinIndex to MaxIndex.
 */

#include <stdio.h>
#include "genlib.h"

/* Constants */

#define MinIndex  0
#define MaxIndex 12

/* Function prototypes */

int Fib(int n);

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
 * This function returns the nth term in the Fibonacci sequence
 * using a recursive implementation of the recurrence relation
 *
 *      Fib(n) = Fib(n - 1) + Fib(n - 2)
 */

int Fib(int n)
{
    if (n < 2) {
        return (n);
    } else {
        return (Fib(n - 1) + Fib(n - 2));
    }
}

