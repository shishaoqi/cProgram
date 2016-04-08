/*
 * File: flipcoin.c
 * ----------------
 * This program simulates flipping a coin.
 */

#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "random.h"

/*
 * Constants
 * ---------
 * NTrials        -- Number of trials
 * EntriesPerLine -- Number of entries on a single line
 */

#define NTrials         100
#define EntriesPerLine   10

/* Private function prototypes */

static string FlipCoin(void);

/* Main program */

main()
{
    int i;

    for (i = 0; i < NTrials; i++) {
        printf("%-7s", FlipCoin());
        if ((i + 1) % EntriesPerLine == 0) printf("\n");
    }
}

/*
 * Function: FlipCoin
 * Usage: str = FlipCoin();
 * ------------------------
 * This function simulates a coin flip by returning
 * either "heads" or "tails" with equal probability.
 */

static string FlipCoin(void)
{
    if (RandomChance(0.50)) {
        return ("heads");
    } else {
        return ("tails");
    }
}
