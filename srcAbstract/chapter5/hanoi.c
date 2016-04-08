/*
 * File: hanoi.c
 * -------------
 * This program solves the Tower of Hanoi problem.
 */

#include <stdio.h>
#include "genlib.h"

/*
 * Constants
 * ---------
 * NDisks -- Number of disks in the original tower
 */

#define NDisks 3

/* Function prototypes */

static void MoveTower(int n, char start, char finish, char temp);
static void MoveSingleDisk(char start, char finish);

/* Main program */

main()
{
    int n;

    MoveTower(NDisks, 'A', 'B', 'C');
}

/*
 * Function: MoveTower
 * Usage: MoveTower(n, start, finish, temp);
 * -----------------------------------------
 * This function is the heart of the recursive solution to the
 * Tower of Hanoi problem.  Calling
 *
 *       MoveTower(n, start, finish, temp)
 *
 * corresponds to the English command "Move a tower of size n
 * from start to finish using temp for intermediate storage."
 */

static void MoveTower(int n, char start, char finish, char temp)
{
    if (n == 1) {
        MoveSingleDisk(start, finish);
    } else {
        MoveTower(n - 1, start, temp, finish);
        MoveSingleDisk(start, finish);
        MoveTower(n - 1, temp, finish, start);
    }
}

/*
 * Function: MoveSingleDisk
 * Usage: MoveSingleDisk(start, finish);
 * -------------------------------------
 * This function encapsulates the operation of moving a single
 * disk.  In this implementation, the function simply displays
 * instructions to the user.  In a more sophisticated program,
 * this function might update a graphical display.
 */

static void MoveSingleDisk(char start, char finish)
{
    printf("%c -> %c\n", start, finish);
}
