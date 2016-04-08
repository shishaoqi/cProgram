/*
 * File: addlist.c
 * ---------------
 * This program adds a list of numbers.  The end of the
 * input is indicated by entering a sentinel value, which
 * is defined by setting the value of the constant Sentinel.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

/*
 * Constants
 * ---------
 * Sentinel -- Value that terminates the input list
 */

#define Sentinel 0

/* Main program */

main()
{
    int value, total;

    printf("This program adds a list of numbers.\n");
    printf("Use %d to signal the end of list.\n", Sentinel);
    total = 0;
    while (TRUE) {
        printf(" ? ");
        value = GetInteger();
        if (value == Sentinel) break;
        total += value;
    }
    printf("The total is %d\n", total);
}
