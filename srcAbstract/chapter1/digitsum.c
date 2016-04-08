/*
 * File: digitsum.c
 * ----------------
 * This program sums the digits in a nonnegative integer.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

/* Function prototypes */

static int DigitSum(int n);

/* Main program */

main()
{
    int n;

    printf("This program sums the digits in an integer.\n");
    printf("Enter a nonnegative integer: ");
    n = GetInteger();
    printf("The sum of the digits is %d\n", DigitSum(n));
}

/*
 * Function: DigitSum
 * Usage: sum = DigitSum(n);
 * -------------------------
 * This function returns the sum of the digits in n, where n
 * is a nonnegative integer.
 */

static int DigitSum(int n)
{
    int sum;

    sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return (sum);
}
