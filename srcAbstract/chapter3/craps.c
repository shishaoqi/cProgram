/*
 * File: craps.c
 * -------------
 * This program plays the casino game called craps, which is
 * played using a pair of dice.  At the beginning of the game,
 * you roll the dice and compute the total.  If your first roll
 * is 7 or 11, you win with what gamblers call a "natural."
 * If your first roll is 2, 3, or 12, you lose by "crapping
 * out."  In any other case, the total from the first roll
 * becomes your "point," after which you continue to roll
 * the dice until one of the following conditions occurs:
 *
 * a) You roll your point again.  This is called "making
 *    your point," which wins.
 *
 * b) You roll a 7, which loses.
 *
 * Other rolls, including 2, 3, 11, and 12, have no effect
 * during this phase of the game.
 */

#include <stdio.h>
#include "genlib.h"
#include "random.h"

/* Function prototypes */

static bool TryToMakePoint(int point);
static int RollTwoDice(void);

/* Main program */

main()
{
    int point;

    Randomize();
    printf("This program plays a game of craps.\n");
    point = RollTwoDice();
    switch (point) {
      case 7: case 11:
        printf("That's a natural.  You win.\n");
        break;
      case 2: case 3: case 12:
        printf("That's craps.  You lose.\n");
        break;
      default:
        printf("Your point is %d.\n", point);
        if (TryToMakePoint(point)) {
            printf("You made your point.  You win.\n");
        } else {
            printf("You rolled a seven.  You lose.\n");
        }
    }
}

/*
 * Function: TryToMakePoint
 * Usage: flag = TryToMakePoint(point);
 * ------------------------------------
 * This function is responsible for the part of the game
 * during which you roll the dice repeatedly until you either
 * make your point or roll a 7.  The function returns TRUE if
 * you make your point and FALSE if a 7 comes up first.
 */

static bool TryToMakePoint(int point)
{
    int total;

    while (TRUE) {
        total = RollTwoDice();
        if (total == point) return (TRUE);
        if (total == 7) return (FALSE);
    }
}

/*
 * Function: RollTwoDice
 * Usage: total = RollTwoDice();
 * -----------------------------
 * This function rolls two dice and returns their sum.  As part
 * of the implementation, the result is displayed on the screen.
 */

static int RollTwoDice(void)
{
    int d1, d2, total;

    printf("Rolling the dice . . .\n");
    d1 = RandomInteger(1, 6);
    d2 = RandomInteger(1, 6);
    total = d1 + d2;
    printf("You rolled %d and %d -- that's %d.\n", d1, d2, total);
    return (total);
}
