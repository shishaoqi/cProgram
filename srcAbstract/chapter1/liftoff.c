/*
 * File: liftoff.c
 * ---------------
 * This program simulates a countdown for a rocket launch.
 */

#include <stdio.h>
#include "genlib.h"

main()
{
    int t;

    for (t = 10; t >= 0; t--) {
        printf("%2d\n", t);
    }
    printf("Liftoff!\n");
}
