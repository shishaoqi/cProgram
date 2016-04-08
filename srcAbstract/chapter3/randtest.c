/*
 * File: randtest.c
 * ----------------
 * This program tests the ANSI rand function.
 */

#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"

/*
 * Constants
 * ---------
 * NTrials -- Number of trials
 */

#define NTrials 10

main()
{
    int i;

    printf("On this computer, RAND_MAX is %d.\n", RAND_MAX);
    printf("The first %d calls to rand return:\n", NTrials);
    for (i = 0; i < NTrials; i++) {
        printf("%10d\n", rand());
    }
}
