/*
 * File: nim.c
 * -----------
 * This program simulates a simple variant of the game of nim.
 * In this version, the game starts with a pile of 13 coins
 * on a table.  Players then take turns removing 1, 2, or 3
 * coins from the pile.  The player who takes the last coin
 * loses.  This simulation allows a human player to compete
 * against the computer.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/*
 * Constants
 * ---------
 * InitialCoins -- Initial number of coins
 * MaxTake      -- The maximum number of coins a player may take
 * NoGoodMove   -- Sentinel indicating no good move is available
 */

#define InitialCoins 13
#define MaxTake       3
#define NoGoodMove   -1

/*
 * Type: playerT
 * -------------
 * This enumeration type distinguishes the turns for the human
 * player from those for the computer.
 */

typedef enum { Human, Computer } playerT;

/* Private function prototypes */

static void GiveInstructions(void);
static void AnnounceWinner(int nCoins, playerT whoseTurn);
static int GetUserMove(int nCoins);
static bool MoveIsLegal(int nTaken, int nCoins);
static int ChooseComputerMove(int nCoins);
static int FindGoodMove(int nCoins);
static bool IsBadPosition(int nCoins);

/*
 * Main program
 * ------------
 * This program plays the game of nim.  In this implementation,
 * the human player always goes first.
 */

main()
{
    int nCoins, nTaken;
    playerT whoseTurn;

    GiveInstructions();
    nCoins = InitialCoins;
    whoseTurn = Human;
    while (nCoins > 1) {
        printf("There are %d coins in the pile.\n", nCoins);
        switch (whoseTurn) {
          case Human:
            nTaken = GetUserMove(nCoins);
            whoseTurn = Computer;
            break;
          case Computer:
            nTaken = ChooseComputerMove(nCoins);
            printf("I'll take %d.\n", nTaken);
            whoseTurn = Human;
            break;
        }
        nCoins -= nTaken;
    }
    AnnounceWinner(nCoins, whoseTurn);
}

/*
 * Function: GiveInstructions
 * Usage: GiveInstructions();
 * --------------------------
 * This function explains the rules of the game to the user.
 */

static void GiveInstructions(void)
{
    printf("Hello.  Welcome to the game of nim.\n");
    printf("In this game, we will start with a pile of\n");
    printf("%d coins on the table.  ", InitialCoins);
    printf("On each turn, you\n");
    printf("and I will alternately take between 1 and\n");
    printf("%d coins from the table.  ", MaxTake);
    printf("The player who\n");
    printf("takes the last coin loses.\n");
    printf("\n");
}

/*
 * Function: AnnounceWinner
 * Usage: AnnounceWinner(nCoins, whoseTurn);
 * -----------------------------------------
 * This function announces the final result of the game.
 */

static void AnnounceWinner(int nCoins, playerT whoseTurn)
{
    if (nCoins == 0) {
        printf("You took the last coin.  You lose.\n");
    } else {
        printf("There is only one coin left.\n");
        switch (whoseTurn) {
          case Human:    printf("I win.\n"); break;
          case Computer: printf("I lose.\n"); break;
        }
    }
}

/*
 * Function: GetUserMove
 * Usage: nTaken = GetUserMove(nCoins);
 * ------------------------------------
 * This function is responsible for the human player's turn.
 * It takes the number of coins left in the pile as an argument,
 * and returns the number of coins that the player removes
 * from the pile.  The function checks the move for legality
 * and gives the player repeated chances to enter a legal move.
 */

static int GetUserMove(int nCoins)
{
    int nTaken, limit;

    while (TRUE) {
        printf("How many would you like? ");
        nTaken = GetInteger();
        if (MoveIsLegal(nTaken, nCoins)) break;
        limit = (nCoins < MaxTake) ? nCoins : MaxTake;
        printf("That's cheating!  Please choose a number");
        printf(" between 1 and %d.\n", limit);
        printf("There are %d coins in the pile.\n", nCoins);
    }
    return (nTaken);
}

/*
 * Function: MoveIsLegal
 * Usage: if (MoveIsLegal(nTaken, nCoins)) . . .
 * ---------------------------------------------
 * This predicate function returns TRUE if it is legal to take
 * nTaken coins from a pile of nCoins.
 */

static bool MoveIsLegal(int nTaken, int nCoins)
{
    return (nTaken > 0 && nTaken <= MaxTake && nTaken <= nCoins);
}

/*
 * Function: ChooseComputerMove
 * Usage: nTaken = ChooseComputerMove(nCoins);
 * -------------------------------------------
 * This function figures out what move is best for the computer
 * player and returns the number of coins taken.  The function
 * first calls FindGoodMove to see if a winning move exists.
 * If none does, the program takes only one coin to give the
 * human player more chances to make a mistake.
 */

static int ChooseComputerMove(int nCoins)
{
    int nTaken;

    nTaken = FindGoodMove(nCoins);
    if (nTaken == NoGoodMove) nTaken = 1;
    return (nTaken);
}

/*
 * Function: FindGoodMove
 * Usage: nTaken = FindGoodMove(nCoins);
 * -------------------------------------
 * This function looks for a winning move, given the specified
 * number of coins.  If there is a winning move in that
 * position, the function returns that value; if not, the
 * function returns the constant NoWinningMove.  This function
 * depends on the recursive insight that a good move is one
 * that leaves your opponent in a bad position and a bad
 * position is one that offers no good moves.
 */

static int FindGoodMove(int nCoins)
{
    int nTaken;

    for (nTaken = 1; nTaken <= MaxTake; nTaken++) {
        if (IsBadPosition(nCoins - nTaken)) return (nTaken);
    }
    return (NoGoodMove);
}

/*
 * Function: IsBadPosition
 * Usage: if (IsBadPosition(nCoins)) . . .
 * ---------------------------------------
 * This function returns TRUE if nCoins is a bad position.
 * A bad position is one in which there is no good move.
 * Being left with a single coin is clearly a bad position
 * and represents the simple case of the recursion.
 */

static bool IsBadPosition(int nCoins)
{
    if (nCoins == 1) return (TRUE);
    return (FindGoodMove(nCoins) == NoGoodMove);
}
